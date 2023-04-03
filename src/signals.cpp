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

	static uint
	copy_frames (Signals* to, const Signals& from, uint from_offset)
	{
		assert(to && *to && from);

		if (from_offset >= from.nsamples())
			return 0;

		      stk::StkFrames* tof   = Signals_get_frames(to);
		const stk::StkFrames* fromf = Signals_get_frames(&from);
		assert(tof && fromf);

		uint nframes =
			std::min(tof->frames() + from_offset, fromf->frames())
			- from_offset;

		for (uint channel = 0; channel < tof->channels(); ++channel)
		{
			uint from_channel = channel < fromf->channels() ? channel : 0;

			for (uint frame = 0; frame < nframes; ++frame)
				(*tof)(frame, channel) = (*fromf)(from_offset + frame, from_channel);
		}

		to->self->nsamples += nframes;
		return nframes;
	}

	static uint
	resample_frames (Signals* to, const Signals& from, uint from_offset)
	{
		assert(to && *to && from);

		if (from_offset >= from.nsamples())
			return 0;

		      stk::StkFrames* tof   = Signals_get_frames(to);
		const stk::StkFrames* fromf = Signals_get_frames(&from);
		assert(tof && fromf);

		float          to_sec = to->capacity()  /  to->sample_rate();
		float        from_sec = from.nsamples() / from.sample_rate();
		float from_offset_sec = from_offset     / from.sample_rate();

		float seconds =
			std::min(to_sec + from_offset_sec, from_sec) - from_offset_sec;

		uint to_nframes = 0, from_nframes = 0;
		if (to_sec + from_offset_sec <= from_sec)
		{
			  to_nframes = to->capacity();
			from_nframes = seconds * from.sample_rate();
		}
		else
		{
			  to_nframes = seconds * to->sample_rate();
			from_nframes = from.nsamples() - from_offset;
		}

		r8b::CFixedBuffer<double> from_buf(from_nframes), to_buf(to_nframes);
		r8b::CDSPResampler24 resampler(
			from.sample_rate(), to->sample_rate(), from_nframes);

		for (uint channel = 0; channel < tof->channels(); ++channel)
		{
			uint from_channel = channel < fromf->channels() ? channel : 0;

			for (uint i = 0; i < from_nframes; ++i)
				from_buf[i] = (*fromf)(from_offset + i, from_channel);

			resampler.clear();
			resampler.oneshot(
				(const double*) from_buf, from_nframes, (double*) to_buf, to_nframes);

			for (uint i = 0; i < to_nframes; ++i)
				(*tof)(i, channel) = to_buf[i];
		}

		to->self->nsamples += to_nframes;
		return from_nframes;
	}

	uint
	Signals_copy (Signals* to, const Signals& from, uint from_offset)
	{
		if (!to || !*to || !from)
			argument_error(__FILE__, __LINE__);

		Signals_set_nsamples(to, 0);

		if (to->sample_rate() == from.sample_rate())
			return copy_frames(to, from, from_offset);
		else
			return resample_frames(to, from, from_offset);
	}

	template <typename T>
	void
	write_buffer (Signals* signals, const SignalBuffer<T>& buffer)
	{
		if (!signals || !*signals)
			argument_error(__FILE__, __LINE__);

		if (
			signals->capacity() < buffer.nsamples() ||
			signals->nchannels() != buffer.nchannels())
		{
			argument_error(__FILE__, __LINE__);
		}

		stk::StkFrames* f = Signals_get_frames(signals);
		assert(f);

		for (uint channel = 0; channel < signals->nchannels(); ++channel)
		{
			uint nsamples = buffer.nsamples();
			const T* buf  = buffer.channel(channel);

			for (uint sample = 0; sample < nsamples; ++sample)
				(*f)(sample, channel) = buf[sample];
		}

		signals->self->nsamples = buffer.nsamples();
	}

	template <>
	void
	Signals_write_buffer (Signals* signals, const SignalBuffer<float>& buffer)
	{
		write_buffer(signals, buffer);
	}

	template <>
	void
	Signals_write_buffer (Signals* signals, const SignalBuffer<double>& buffer)
	{
		write_buffer(signals, buffer);
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
		return f && f->frames() > 0 && f->channels() > 0;
	}

	bool
	Signals::operator ! () const
	{
		return !operator bool();
	}


}// Beeps
