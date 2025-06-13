#include "signals.h"


#include <assert.h>
#include <memory>
#include <algorithm>
#include <AudioFile.h>
#include <CDSPResampler.h>
#include "beeps/exception.h"


namespace Beeps
{


	class Frames : public stk::StkFrames
	{

		typedef stk::StkFrames Super;

		public:

			Frames (
				unsigned int nframes   = 0,
				unsigned int nchannels = 1,
				double sample_rate     = 0)
			:	Super(nframes, nchannels)
			{
				if (sample_rate == 0)
					sample_rate = Beeps::sample_rate();

				if (nframes <= 0)
					argument_error(__FILE__, __LINE__);
				if (nchannels <= 0)
					argument_error(__FILE__, __LINE__);
				if (sample_rate <= 0)
					argument_error(__FILE__, __LINE__);

				setDataRate(sample_rate);
			}

			Frames (const Frames& obj)
			:	Super(obj)
			{
				setDataRate(obj.dataRate());
			}

			void slice (uint start_, uint end_)
			{
				assert(!saved_data_);

				size_t start  = start_;
				size_t end    = end_;
				if (start > nFrames_ || end > nFrames_)
					argument_error(__FILE__, __LINE__);

				saved_data_       = data_;
				saved_nFrames_    = nFrames_;
				saved_size_       = size_;
				saved_bufferSize_ = bufferSize_;

				data_      += start * nChannels_;
				nFrames_    = end - start;
				size_       = nFrames_ * nChannels_;
				bufferSize_ = size_;
			}

			void unslice ()
			{
				assert(saved_data_);

				data_       = saved_data_;
				nFrames_    = saved_nFrames_;
				size_       = saved_size_;
				bufferSize_ = saved_bufferSize_;

				saved_data_       = NULL;
				saved_nFrames_    = 0;
				saved_size_       = 0;
				saved_bufferSize_ = 0;
			}

			const Sample* data () const
			{
				return data_;
			}

		private:

			Sample* saved_data_ = NULL;

			size_t saved_nFrames_ = 0, saved_size_ = 0, saved_bufferSize_ = 0;

	};// Frames


	struct Signals::Data
	{

		std::unique_ptr<Frames> frames;

		uint nsamples = 0;

	};// Signals::Data


	Signals
	Signals_create (uint capacity, uint nchannels, double sample_rate)
	{
		Signals s;
		s.self->frames.reset(
			new Frames(capacity * nchannels, nchannels, sample_rate));
		return s;
	}

	Signals
	Signals_create (
		const float* const* channels,
		uint nsamples, uint nchannels, double sample_rate, uint capacity)
	{
		if (!channels)
			argument_error(__FILE__, __LINE__);

		Signals s = Signals_create(
			capacity > nsamples ? capacity : nsamples, nchannels, sample_rate);

		for (uint channel = 0; channel < nchannels; ++channel)
		{
			Sample* p = Signals_at(&s, 0, channel);
			for (uint i = 0; i < nsamples; ++i, p += nchannels)
				*p = channels[channel][i];
		}

		s.self->nsamples = nsamples;
		return s;
	}

	void
	Signals_clear (Signals* signals)
	{
		if (!signals)
			argument_error(__FILE__, __LINE__);

		signals->self->nsamples = 0;
	}

	void
	Signals_clear (Signals* signals, uint capacity)
	{
		if (!signals)
			argument_error(__FILE__, __LINE__);
		if (!*signals)
			argument_error(__FILE__, __LINE__);
		if (capacity <= 0)
			argument_error(__FILE__, __LINE__);

		Signals_clear(signals);
		signals->self->frames->resize(capacity, signals->nchannels());
	}

	uint
	Signals_tick (Signals* signals, std::function<void(stk::StkFrames*)> fun)
	{
		if (!signals)
			argument_error(__FILE__, __LINE__);
		if (!*signals)
			argument_error(__FILE__, __LINE__);

		fun(signals->self->frames.get());
		Signals_set_nsamples(signals, signals->capacity());
		return signals->nsamples();
	}

	uint
	Signals_tick (
		Signals* output, const Signals& input,
		std::function<void(stk::StkFrames*, const stk::StkFrames&)> fun)
	{
		if (!output)
			argument_error(__FILE__, __LINE__);
		if (!*output)
			argument_error(__FILE__, __LINE__);
		if (!input)
			argument_error(__FILE__, __LINE__);

		Signals_clear(output, input.nsamples());
		Signals_set_nsamples(output, output->capacity());

		fun(output->self->frames.get(), *input.self->frames);
		return output->nsamples();
	}

	uint
	Signals_tick (
		Signals* signals, uint start, uint end,
		std::function<void(stk::StkFrames*)> fun)
	{
		if (!signals)
			argument_error(__FILE__, __LINE__);
		if (!*signals)
			argument_error(__FILE__, __LINE__);
		if (start > end)
			argument_error(__FILE__, __LINE__);

		if (start == 0 && end == signals->capacity())
			return Signals_tick(signals, fun);

		signals->self->frames->slice(start, end);
		fun(signals->self->frames.get());
		signals->self->frames->unslice();
		Signals_set_nsamples(signals, end);
		return end;
	}

	void
	Signals_fill (Signals* signals, uint nsamples, Sample value, uint offset)
	{
		if (!signals)
			argument_error(__FILE__, __LINE__);
		if (!*signals)
			argument_error(__FILE__, __LINE__);

		uint new_nsamples = offset + nsamples;
		if (new_nsamples > signals->capacity())
			beeps_error(__FILE__, __LINE__);

		Sample* p   = Signals_at(signals, offset);
		size_t size = nsamples * signals->nchannels();
		for (size_t i = 0; i < size; ++i)
			*p++ = value;

		Signals_set_nsamples(signals, new_nsamples);
	}

	void
	Signals_shift (Signals* signals, uint nsamples)
	{
		if (!signals)
			argument_error(__FILE__, __LINE__);
		if (!*signals)
			argument_error(__FILE__, __LINE__);

		if (nsamples == 0)
			return;

		int new_nsamples = signals->nsamples() - nsamples;
		if (new_nsamples < 0)
			new_nsamples = 0;

		Sample* from = Signals_at(signals, nsamples);
		Sample* to   = Signals_at(signals, 0);
		int size     = new_nsamples * signals->nchannels();
		for (int i = 0; i < size; ++i)
			*to++ = *from++;

		Signals_set_nsamples(signals, new_nsamples);
	}

	static uint
	copy_samples (Signals* to, const Signals& from, uint from_offset)
	{
		assert(to && *to && from);

		if (from_offset >= from.nsamples())
			return 0;

		uint   to_nchannels = to->nchannels();
		uint from_nchannels = from.nchannels();
		uint   to_offset    = to->nsamples();
		uint   to_nsamples  = to->capacity() - to_offset;
		uint from_nsamples  = from.nsamples();
		uint copy_nsamples  =
			std::min(from_offset + to_nsamples, from_nsamples) - from_offset;

		for (uint channel = 0; channel < to_nchannels; ++channel)
		{
			uint from_channel    = channel < from_nchannels ? channel : 0;
			      Sample*   to_p = Signals_at(to,     to_offset,      channel);
			const Sample* from_p = Signals_at(from, from_offset, from_channel);
			for (uint i = 0; i < copy_nsamples; ++i)
			{
				 *to_p  = *from_p;
				  to_p +=    to_nchannels;
				from_p +=  from_nchannels;
			}
		}

		Signals_set_nsamples(to, to->self->nsamples + copy_nsamples);
		return copy_nsamples;
	}

	static uint
	resample (Signals* to, const Signals& from, uint from_offset)
	{
		assert(to && *to && from);

		if (from_offset >= from.nsamples())
			return 0;

		uint    to_offset     = to->nsamples();
		float          to_sec = (to->capacity() - to_offset) /  to->sample_rate();
		float from_offset_sec = from_offset                  / from.sample_rate();
		float        from_sec = from.nsamples()              / from.sample_rate();

		float copy_seconds =
			std::min(from_offset_sec + to_sec, from_sec) - from_offset_sec;

		uint to_nsamples = 0, from_nsamples = 0;
		if (from_offset_sec + to_sec <= from_sec)
		{
			  to_nsamples = to->capacity() - to->nsamples();
			from_nsamples = copy_seconds * from.sample_rate();
		}
		else
		{
			to_nsamples = copy_seconds * to->sample_rate();
			from_nsamples = from.nsamples() - from_offset;
		}

		r8b::CDSPResampler24 resampler(
			from.sample_rate(), to->sample_rate(), from_nsamples);
		r8b::CFixedBuffer<double> from_buf(from_nsamples), to_buf(to_nsamples);

		for (uint channel = 0; channel < to->nchannels(); ++channel)
		{
			uint from_channel = channel < from.nchannels() ? channel : 0;

			for (uint i = 0; i < from_nsamples; ++i)
				from_buf[i] = *Signals_at(from, from_offset + i, from_channel);

			resampler.clear();
			resampler.oneshot(
				(const double*) from_buf, from_nsamples,
				      (double*)   to_buf,   to_nsamples);

			for (uint i = 0; i < to_nsamples; ++i)
				*Signals_at(to, to_offset + i, channel) = to_buf[i];
		}

		Signals_set_nsamples(to, to->self->nsamples + to_nsamples);
		return from_nsamples;
	}

	uint
	Signals_copy (Signals* to, const Signals& from, uint from_offset)
	{
		if (!to)
			argument_error(__FILE__, __LINE__);
		if (!*to)
			argument_error(__FILE__, __LINE__);
		if (!from)
			argument_error(__FILE__, __LINE__);

		Signals_clear(to);

		if (to->sample_rate() == from.sample_rate())
			return copy_samples(to, from, from_offset);
		else
			return resample(to, from, from_offset);
	}

	void
	Signals_add (Signals* signals, const Signals& add)
	{
		if (!signals)
			argument_error(__FILE__, __LINE__);
		if (signals->nchannels() != add.nchannels())
			argument_error(__FILE__, __LINE__);

		      Sample* sig_p = Signals_at(signals, 0);
		const Sample* add_p = Signals_at(add,     0);
		uint nsamples = std::min(signals->nsamples(), add.nsamples());
		uint size     = nsamples * signals->nchannels();

		for (uint i = 0; i < size; ++i, ++sig_p, ++add_p)
			*sig_p += *add_p;
	}

	void
	Signals_multiply (Signals* signals, const Signals& multiplier)
	{
		if (!signals)
			argument_error(__FILE__, __LINE__);
		if (multiplier.nchannels() != 1)
			argument_error(__FILE__, __LINE__);
		if (signals->capacity() != multiplier.capacity())
			argument_error(__FILE__, __LINE__);

		uint nchannels = signals->nchannels();
		uint nsamples  = signals->nsamples();

		for (uint ch = 0; ch < nchannels; ++ch)
		{
			      Sample* sig_p = Signals_at(signals,    0, ch);
			const Sample* mul_p = Signals_at(multiplier, 0, 0);
			for (uint i = 0; i < nsamples; ++i, sig_p += nchannels, ++mul_p)
				*sig_p *= *mul_p;
		}
	}

	void
	Signals_offset_and_scale (
		Signals* signals, float offset, float scale,
		int channel, uint start, int end_)
	{
		if (!signals)
			argument_error(__FILE__, __LINE__);

		uint signals_nsamples = signals->nsamples();
		uint end              = end_ < 0 ? signals_nsamples : (uint) end_;
		if (start > signals_nsamples)
			argument_error(__FILE__, __LINE__);
		if (end   > signals_nsamples)
			argument_error(__FILE__, __LINE__);
		if (end < start)
			argument_error(__FILE__, __LINE__);

		if (offset == 0 && scale == 1) return;

		uint nchannels = signals->nchannels();
		uint nsamples  = end - start;

		if (channel >= 0)
		{
			Sample* p = Signals_at(signals, start, channel);
			for (uint i = 0; i < nsamples; ++i, p += nchannels)
				*p = *p * scale + offset;
		}
		else
		{
			uint size = nchannels * nsamples;
			Sample* p = Signals_at(signals, start, 0);
			for (uint i = 0; i < size; ++i, ++p)
				*p = *p * scale + offset;
		}
	}

	template <typename T>
	void
	write_samples (Signals* signals, const SignalSamples<T>& samples, long nsamples_)
	{
		uint nchannels = signals->nchannels();
		uint nsamples  = nsamples_ < 0 ? samples.nsamples() : (uint) nsamples_;

		if (!signals)
			argument_error(__FILE__, __LINE__);
		if (!*signals)
			argument_error(__FILE__, __LINE__);
		if (signals->nchannels() != samples.nchannels())
			argument_error(__FILE__, __LINE__);
		if (signals->capacity() < nsamples)
			argument_error(__FILE__, __LINE__);

		for (uint channel = 0; channel < nchannels; ++channel)
		{
			Sample* sig_p = Signals_at(signals, 0, channel);
			const T* buf  = samples.channel(channel);
			for (uint i = 0; i < nsamples; ++i, sig_p += nchannels)
				*sig_p = buf[i];
		}

		Signals_set_nsamples(signals, nsamples);
	}

	template <>
	void
	Signals_write_samples (
		Signals* signals, const SignalSamples<float>& samples, long nsamples)
	{
		write_samples(signals, samples, nsamples);
	}

	template <>
	void
	Signals_write_samples (
		Signals* signals, const SignalSamples<double>& samples, long nsamples)
	{
		write_samples(signals, samples, nsamples);
	}

	void
	Signals_set_capacity (Signals* signals, uint capacity)
	{
		if (!signals)
			argument_error(__FILE__, __LINE__);
		if (!*signals)
			argument_error(__FILE__, __LINE__);
		if (capacity == 0)
			argument_error(__FILE__, __LINE__);

		if (capacity == signals->capacity())
			return;

		uint nsamples  = signals->nsamples();
		uint nchannels = signals->nchannels();

		if (capacity < signals->capacity())
		{
			signals->self->frames->resize(capacity, nchannels);
			if (capacity < nsamples) Signals_set_nsamples(signals, capacity);
		}
		else
		{
			double sample_rate = signals->sample_rate();
			const Sample* from = Signals_at(signals, 0);

			std::unique_ptr<Frames> old = std::move(signals->self->frames);
			signals->self->frames.reset(
				new Frames(capacity * nchannels, nchannels, sample_rate));

			Sample* to = Signals_at(signals, 0);
			uint size  = nsamples * nchannels;
			for (uint i = 0; i < size; ++i)
				*to++ = *from++;
		}
	}

	void
	Signals_set_nsamples (Signals* signals, uint nsamples)
	{
		if (!signals)
			argument_error(__FILE__, __LINE__);
		if (nsamples > signals->capacity())
			argument_error(__FILE__, __LINE__);

		signals->self->nsamples = nsamples;
	}

	Sample*
	Signals_at (Signals* signals, uint index, uint channel)
	{
		if (!signals)
			argument_error(__FILE__, __LINE__);
		if (!*signals)
			argument_error(__FILE__, __LINE__);

		return &(*signals->self->frames)(index, channel);
	}

	const Sample*
	Signals_at (const Signals* signals, uint index, uint channel)
	{
		return Signals_at(const_cast<Signals*>(signals), index, channel);
	}

	const Sample*
	Signals_at (const Signals& signals, uint index, uint channel)
	{
		return Signals_at(const_cast<Signals*>(&signals), index, channel);
	}

	float
	Signals_get_seconds (const Signals& signals)
	{
		return (float) (signals.nsamples() / signals.sample_rate());
	}

	static void
	make_audio_buffer (
		AudioFile<float>::AudioBuffer* buffer, const Signals& signals)
	{
		assert(buffer && signals);

		uint nchannels = signals.nchannels();
		uint nsamples  = signals.nsamples();

		buffer->clear();
		for (uint ch = 0; ch < nchannels; ++ch)
		{
			buffer->emplace_back(std::vector<float>());
			auto& channel = buffer->back();

			channel.reserve(nsamples);

			const Sample* p = Signals_at(signals, 0, ch);
			for (uint i = 0; i < nsamples; ++i, p += nchannels)
				channel.push_back(*p);
		}
	}

	void
	Signals_save (const Signals& signals, const char* path)
	{
		AudioFile<float> file;
		file.setSampleRate(signals.sample_rate());
		make_audio_buffer(&file.samples, signals);
		file.save(path);
	}


	Signals::Signals ()
	{
	}

	Signals::~Signals ()
	{
	}

	Signals
	Signals::dup () const
	{
		Signals t;
		if (self->frames)
			t.self->frames.reset(new Frames(*self->frames));
		t.self->nsamples = self->nsamples;
		return t;
	}

	double
	Signals::sample_rate () const
	{
		return self->frames ? self->frames->dataRate() : Beeps::sample_rate();
	}

	uint
	Signals::nchannels () const
	{
		return self->frames ? self->frames->channels() : 0;
	}

	uint
	Signals::nsamples () const
	{
		return self->nsamples;
	}

	uint
	Signals::capacity () const
	{
		return self->frames ? self->frames->frames() : 0;
	}

	bool
	Signals::empty () const
	{
		return nsamples() == 0;
	}

	bool
	Signals::full () const
	{
		uint n = nsamples();
		if (n == 0) return false;

		return n == capacity();
	}

	const Sample*
	Signals::samples () const
	{
		return self->frames ? self->frames->data() : NULL;
	}

	Signals::operator bool () const
	{
		const Frames* f = self->frames.get();
		return f && f->dataRate() > 0 && f->channels() > 0 && f->frames() > 0;
	}

	bool
	Signals::operator ! () const
	{
		return !operator bool();
	}


}// Beeps
