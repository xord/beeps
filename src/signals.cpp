#include "signals.h"


#include <memory>
#include <algorithm>
#include <CDSPResampler.h>
#include "beeps/beeps.h"
#include "beeps/exception.h"


namespace Beeps
{


	struct Signals::Data
	{

		std::unique_ptr<stk::StkFrames> frames;

		uint nsamples = 0;

	};// Signals::Data


	stk::StkFrames*
	Signals_get_frames (Signals* signals)
	{
		if (!signals)
			argument_error(__FILE__, __LINE__);

		return signals->self->frames.get();
	}

	const stk::StkFrames*
	Signals_get_frames (const Signals* signals)
	{
		return Signals_get_frames(const_cast<Signals*>(signals));
	}

	Signals
	Signals_create (uint capacity, uint nchannels, double sample_rate)
	{
		if (capacity <= 0 || nchannels <= 0)
			argument_error(__FILE__, __LINE__);

		if (sample_rate <= 0) sample_rate = Beeps::sample_rate();

		Signals s;
		s.self->frames.reset(new stk::StkFrames(capacity, nchannels));
		s.self->frames->setDataRate(sample_rate);
		return s;
	}

	Signals
	Signals_create (
		const float* const* channels,
		uint nsamples, uint nchannels, double sample_rate)
	{
		if (!channels || nsamples <= 0 || nchannels <= 0)
			argument_error(__FILE__, __LINE__);

		if (sample_rate <= 0) sample_rate = Beeps::sample_rate();

		stk::StkFrames* frames = new stk::StkFrames(nsamples, nchannels);
		frames->setDataRate(sample_rate);

		for (uint channel = 0; channel < nchannels; ++channel)
			for (uint sample = 0; sample < nsamples; ++sample)
				(*frames)(sample, channel) = channels[channel][sample];

		Signals s;
		s.self->frames.reset(frames);
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

	static uint
	copy_frames (Signals* to, const Signals& from, uint from_offset)
	{
		assert(to && *to && from);

		if (from_offset >= from.nsamples())
			return 0;

		uint   to_offset   = to->nsamples();
		uint   to_nsamples = to->capacity() - to_offset;
		uint from_nsamples = from.nsamples();
		uint copy_nsamples =
			std::min(from_offset + to_nsamples, from_nsamples)
			- from_offset;

		      stk::StkFrames*   to_frames = Signals_get_frames(to);
		const stk::StkFrames* from_frames = Signals_get_frames(&from);
		assert(to_frames && from_frames);

		for (uint channel = 0; channel < to_frames->channels(); ++channel)
		{
			uint from_channel = channel < from_frames->channels() ? channel : 0;

			for (uint i = 0; i < copy_nsamples; ++i)
			{
				(*to_frames)(      to_offset + i,      channel) =
					(*from_frames)(from_offset + i, from_channel);
			}
		}

		to->self->nsamples += copy_nsamples;
		return copy_nsamples;
	}

	static uint
	resample_frames (Signals* to, const Signals& from, uint from_offset)
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

		      stk::StkFrames*   to_frames = Signals_get_frames(to);
		const stk::StkFrames* from_frames = Signals_get_frames(&from);
		assert(to_frames && from_frames);

		for (uint channel = 0; channel < to_frames->channels(); ++channel)
		{
			uint from_channel = channel < from_frames->channels() ? channel : 0;

			for (uint i = 0; i < from_nsamples; ++i)
				from_buf[i] = (*from_frames)(from_offset + i, from_channel);

			resampler.clear();
			resampler.oneshot(
				(const double*) from_buf, from_nsamples,
				      (double*)   to_buf,   to_nsamples);

			for (uint i = 0; i < to_nsamples; ++i)
				(*to_frames)(to_offset + i, channel) = to_buf[i];
		}

		to->self->nsamples += to_nsamples;
		return from_nsamples;
	}

	uint
	Signals_copy (Signals* to, const Signals& from, uint from_offset)
	{
		if (!to || !*to || !from)
			argument_error(__FILE__, __LINE__);

		Signals_clear(to);

		if (to->sample_rate() == from.sample_rate())
			return copy_frames(to, from, from_offset);
		else
			return resample_frames(to, from, from_offset);
	}

	template <typename T>
	void
	write_samples (Signals* signals, const SignalSamples<T>& samples, long nsamples_)
	{
		uint nsamples = nsamples_ < 0 ? samples.nsamples() : (uint) nsamples_;

		if (
			!signals || !*signals ||
			signals->nchannels() != samples.nchannels() ||
			signals->capacity() < nsamples)
		{
			argument_error(__FILE__, __LINE__);
		}

		stk::StkFrames* f = Signals_get_frames(signals);
		assert(f);

		for (uint channel = 0; channel < signals->nchannels(); ++channel)
		{
			const T* buf = samples.channel(channel);
			for (uint sample = 0; sample < nsamples; ++sample)
				(*f)(sample, channel) = buf[sample];
		}

		signals->self->nsamples = nsamples;
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
	Signals_set_nsamples (Signals* signals, uint nsamples)
	{
		if (!signals)
			argument_error(__FILE__, __LINE__);

		signals->self->nsamples = nsamples;
	}

	float
	Signals_get_seconds (const Signals& signals)
	{
		return (float) (signals.nsamples() / signals.sample_rate());
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
			t.self->frames.reset(new stk::StkFrames(*self->frames));
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

	Signals::operator bool () const
	{
		const stk::StkFrames* f = self->frames.get();
		return f && f->dataRate() > 0 && f->channels() > 0 && f->frames() > 0;
	}

	bool
	Signals::operator ! () const
	{
		return !operator bool();
	}


}// Beeps
