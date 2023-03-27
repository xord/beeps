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
	Signals_create (
		float seconds, uint nchannels, uint sampling_rate)
	{
		Signals s;
		if (seconds > 0 && nchannels > 0)
		{
			s.self->frames.reset(
				new stk::StkFrames(seconds * s.sampling_rate(), nchannels));

			if (sampling_rate > 0)
				s.self->frames->setDataRate(sampling_rate);
		}
		return s;
	}

	Signals
	Signals_create (
		const float* const* channels,
		uint nsamples, uint nchannels, uint sampling_rate)
	{
		Signals s;
		if (channels && nsamples > 0 && nchannels > 0)
		{
			stk::StkFrames* frames = new stk::StkFrames(nsamples, nchannels);
			for (uint channel = 0; channel < nchannels; ++channel)
				for (uint sample = 0; sample < nsamples; ++sample)
					(*frames)(sample, channel) = channels[channel][sample];
			s.self->frames.reset(frames);

			if (sampling_rate > 0)
				s.self->frames->setDataRate(sampling_rate);
		}
		return s;
	}

	template <typename T>
	void
	set_buffer (
		Signals* signals, const SignalBuffer<T>& buffer)
	{
		if (!signals)
			argument_error(__FILE__, __LINE__);

		stk::StkFrames* f = Signals_get_frames(signals);
		if (!f)
			invalid_state_error(__FILE__, __LINE__);

		f->resize(buffer.nsamples(), buffer.nchannels());

		for (uint channel = 0; channel < buffer.nchannels(); ++channel)
		{
			const T* buf = buffer.channel(channel);
			for (uint sample = 0; sample < buffer.nsamples(); ++sample)
				(*f)(sample, channel) = buf[sample];
		}
	}

	template <>
	void
	Signals_set_buffer (Signals* signals, const SignalBuffer<float>& buffer)
	{
		set_buffer(signals, buffer);
	}

	template <>
	void
	Signals_set_buffer (Signals* signals, const SignalBuffer<double>& buffer)
	{
		set_buffer(signals, buffer);
	}

	static bool
	copy (Signals* to, const Signals& from, float from_offset_sec)
	{
		assert(to && *to && from);

		      stk::StkFrames* tof   = Signals_get_frames(to);
		const stk::StkFrames* fromf = Signals_get_frames(&from);
		if (!tof || !fromf) return false;

		uint offset = from_offset_sec * from.sampling_rate();
		uint frames = std::min(fromf->frames(), offset + tof->frames());

		for (uint channel = 0; channel < tof->channels(); ++channel)
		{
			uint nframes      = frames - offset;
			uint from_channel = channel < fromf->channels() ? channel : 0;

			for (uint frame = 0; frame < nframes; ++frame)
				(*tof)(frame, channel) = (*fromf)(offset + frame, from_channel);
		}

		return true;
	}

	static bool
	copy_interpolate (
		Signals* to, const Signals& from, float from_offset_sec)
	{
		assert(to && *to && from);

		      stk::StkFrames* tof   = Signals_get_frames(to);
		const stk::StkFrames* fromf = Signals_get_frames(&from);
		if (!tof || !fromf) return false;

		float seconds =
			std::min(from_offset_sec + to->seconds(), from.seconds())
			- from_offset_sec;

		uint from_offset  = from_offset_sec * from.sampling_rate();
		uint from_nframes = seconds         * from.sampling_rate();
		uint   to_nframes = seconds         * to->sampling_rate();

		for (uint channel = 0; channel < tof->channels(); ++channel)
		{
			uint from_channel = channel < fromf->channels() ? channel : 0;

			r8b::CDSPResampler24 resampler(
				from.sampling_rate(), to->sampling_rate(), from_nframes);

			r8b::CFixedBuffer<double> frombuf(from_nframes);
			for (uint i = 0; i < from_nframes; ++i)
				frombuf[i] = (*fromf)(from_offset + i, from_channel);

			r8b::CFixedBuffer<double> tobuf(to_nframes);
			resampler.oneshot(
				(const double*) frombuf, from_nframes, (double*) tobuf, to_nframes);

			for (uint i = 0; i < to_nframes; ++i)
				(*tof)(i, channel) = tobuf[i];
		}

		return true;
	}

	bool
	Signals_copy (Signals* to, const Signals& from, float from_offset_sec)
	{
		if (!to || !*to || !from || from_offset_sec < 0)
			argument_error(__FILE__, __LINE__);

		to->clear();

		if (to->sampling_rate() == from.sampling_rate())
			return copy(to, from, from_offset_sec);
		else
			return copy_interpolate(to, from, from_offset_sec);
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
		return t;
	}

	void
	Signals::clear ()
	{
		stk::StkFrames* f = self->frames.get();
		if (!f) return;

		size_t size = f->size();
		for (size_t i = 0; i < size; ++i)
			(*f)[i] = 0;
	}

	uint
	Signals::sampling_rate () const
	{
		Data* p = self.get();
		return p->frames ? p->frames->dataRate() : Beeps::sampling_rate();
	}

	uint
	Signals::nsamples () const
	{
		Data* p = self.get();
		return p->frames ? p->frames->frames() : 0;
	}

	uint
	Signals::nchannels () const
	{
		Data* p = self.get();
		return p->frames ? p->frames->channels() : 0;
	}

	float
	Signals::seconds () const
	{
		double sec = nsamples() / (double) sampling_rate();
		return (float) sec;
	}

	Signals::operator bool () const
	{
		const stk::StkFrames* f = Signals_get_frames(this);
		return f && f->frames() > 0 && f->channels() > 0;
	}

	bool
	Signals::operator ! () const
	{
		return !operator bool();
	}


}// Beeps
