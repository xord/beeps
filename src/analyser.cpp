#include "beeps/filter.h"


#include <math.h>
#include <assert.h>
#include <algorithm>
#include "pffft.h"
#include "beeps/beeps.h"
#include "beeps/exception.h"
#include "signals.h"


namespace Beeps
{


	struct PFFFTDestroySetup
	{
		void operator () (PFFFT_Setup* ptr)
		{
			pffft_destroy_setup(ptr);
		}
	};

	struct PFFFTAlignedFree
	{
		void operator () (float* ptr)
		{
			pffft_aligned_free(ptr);
		}
	};


	struct Analyser::Data
	{

		Signals signals;

		Spectrum spectrum;

		std::unique_ptr<PFFFT_Setup, PFFFTDestroySetup> pffft;

		std::unique_ptr<float[], PFFFTAlignedFree> fft_buffer;

		uint fft_size = 0;

		~Data ()
		{
			clear();
		}

		void setup (uint size)
		{
			if (size == fft_size) return;

			clear();

			signals   = Signals_create(std::min(size, (uint) Beeps::sample_rate()));
			pffft     .reset(pffft_new_setup(size, PFFFT_REAL));
			fft_buffer.reset((float*) pffft_aligned_malloc(sizeof(float) * size));
			fft_size  = size;
		}

		void clear ()
		{
			if (!pffft) return;

			signals   = Signals();
			spectrum  .clear();
			pffft     .reset();
			fft_buffer.reset();
			fft_size  = 0;
		}

		bool is_valid () const
		{
			return signals && pffft && fft_buffer && fft_size > 0;
		}

	};// Analyser::Data


	Analyser::Analyser (uint fft_size, Processor* input)
	:	Super(input)
	{
		set_fft_size(fft_size);
	}

	Analyser::~Analyser ()
	{
	}

	static bool
	is_valid_fft_size (uint size)
	{
		if (size < 32) return false;

		while (size > 1)
		{
			     if (size % 2 == 0) size /= 2;
			else if (size % 3 == 0) size /= 3;
			else if (size % 5 == 0) size /= 5;
			else break;
		}
		return size == 1;
	}

	void
	Analyser::set_fft_size (uint size)
	{
		if (!is_valid_fft_size(size))
			beeps_error(__FILE__, __LINE__, "fft_size must be divisible by 2, 3, or 5");

		self->setup(size);
	}

	uint
	Analyser::fft_size () const
	{
		return self->fft_size;
	}

	float
	Analyser::resolution () const
	{
		if (self->fft_size == 0) return 0;
		return self->signals.sample_rate() / self->fft_size;
	}

	const Signals&
	Analyser::signals () const
	{
		return self->signals;
	}

	static void
	copy_signals_to_fft_buffer (float* buffer, uint size, const Signals& signals)
	{
		const double* samples = signals.samples();
		uint nsamples         = size > signals.nsamples() ? signals.nsamples() : size;

		switch (signals.nchannels())
		{
			case 1:
				for (uint i = 0; i < nsamples; ++i)
					buffer[i] = samples[i];
				break;

			case 2:
				for (uint i = 0; i < nsamples; ++i, samples += 2)
					buffer[i] = (samples[0] + samples[1]) * 0.5;
				break;

			default:
				beeps_error(__FILE__, __LINE__);
		}

		for (uint i = nsamples; i < size; ++i)
			buffer[i] = 0;
	}

	static void
	update_spectrum (Analyser::Spectrum* spectrum, float* buffer, uint buffer_size)
	{
		assert(spectrum && spectrum->empty() && buffer && (buffer_size % 2) == 0);

		uint size  = buffer_size / 2;
		float div = 1.0 / size;
		float* p  = buffer;

		spectrum->reserve(size);
		for (uint i = 0; i < size; ++i, p += 2)
			spectrum->push_back(sqrt(p[0] * p[0] + p[1] * p[1]) * div);
	}

	const Analyser::Spectrum&
	Analyser::spectrum () const
	{
		if (self->spectrum.empty() && *this)
		{
			float* p = &self->fft_buffer[0];
			copy_signals_to_fft_buffer(p, self->fft_size, self->signals);
			pffft_transform_ordered(self->pffft.get(), p, p, NULL, PFFFT_FORWARD);
			update_spectrum(&self->spectrum, p, self->fft_size);
		}
		return self->spectrum;
	}

	Analyser::operator bool () const
	{
		if (!Super::operator bool()) return false;
		return self->is_valid();
	}

	static void
	shift (Signals* signals, uint nsamples)
	{
		if (nsamples == 0)
			return;

		if (nsamples > signals->nsamples())
			return Signals_clear(signals);

		Sample* from         = Signals_at(signals, nsamples);
		Sample* to           = Signals_at(signals, 0);
		uint nsamples_remain = signals->nsamples() - nsamples;
		uint size            = nsamples_remain * signals->nchannels();
		for (uint i = 0; i < size; ++i)
			*to++ = *from++;

		Signals_set_nsamples(signals, nsamples_remain);
	}

	static void
	append (Signals* to, const Signals& from)
	{
		assert(to->nchannels() == from.nchannels());
		assert(to->nsamples() + from.nsamples() <= to->capacity());

		uint nchannels       =  to->nchannels();
		uint nsamples        = std::min(from.nsamples(), to->capacity() - to->nsamples());
		uint size            = nsamples * nchannels;
		      Sample*   to_p = Signals_at(to,   to->nsamples());
		const Sample* from_p = Signals_at(from, 0);
		for (uint i = 0; i < size; ++i)
			*to_p++ = *from_p++;

		Signals_set_nsamples(to, to->nsamples() + nsamples);
	}

	void
	Analyser::filter (Context* context, Signals* signals, uint* offset)
	{
		Super::filter(context, signals, offset);

		auto& in =      *signals;
		auto& my = self->signals;

		if (my.nchannels() != in.nchannels() || my.sample_rate() != in.sample_rate())
			my = Signals_create(my.capacity(), in.nchannels(), in.sample_rate());

		uint total_nsamples = my.nsamples() + in.nsamples();
		if (total_nsamples > my.capacity())
			shift(&my, total_nsamples - my.capacity());

		append(&my, in);

		self->spectrum.clear();
	}


}// Beeps
