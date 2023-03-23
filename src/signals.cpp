#include "signals.h"


#include <memory>
#include <algorithm>
#include "beeps/beeps.h"
#include "beeps/exception.h"


namespace Beeps
{


	struct Signals::Data
	{

		uint sampling_rate = 0;

		std::unique_ptr<stk::StkFrames> frames;

	};// Signals::Data


	Signals
	Signals_create (
		float seconds, uint nchannels, uint sampling_rate)
	{
		Signals s;
		s.self->sampling_rate = sampling_rate;

		if (seconds > 0 && nchannels > 0)
		{
			s.self->frames.reset(
				new stk::StkFrames(seconds * s.sampling_rate(), nchannels));
		}

		return s;
	}

	Signals
	Signals_create (
		const float* const* channels,
		uint nsamples, uint nchannels, uint sampling_rate)
	{
		Signals s;
		s.self->sampling_rate = sampling_rate;

		if (channels && nsamples > 0 && nchannels > 0)
		{
			stk::StkFrames* frames = new stk::StkFrames(nsamples, nchannels);
			for (uint channel = 0; channel < nchannels; ++channel)
				for (uint sample = 0; sample < nsamples; ++sample)
					(*frames)(sample, channel) = channels[channel][sample];
			s.self->frames.reset(frames);
		}

		return s;
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

		float seconds = std::min(from_offset_sec + to->seconds(), from.seconds());

		uint from_frames   = fromf->frames();
		uint from_offset   = from_offset_sec * from.sampling_rate();
		float to2from_rate = from.sampling_rate() / (float) to->sampling_rate();

		for (uint channel = 0; channel < tof->channels(); ++channel)
		{
			uint nframes      = (seconds - from_offset_sec) * to->sampling_rate();
			uint from_channel = channel < fromf->channels() ? channel : 0;

			for (uint frame = 0; frame < nframes; ++frame)
			{
				stk::StkFloat from_frame = from_offset + frame * to2from_rate;
				if (from_frame >= from_frames) continue;

				(*tof)(frame, channel) = fromf->interpolate(from_frame, from_channel);
			}
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
		return p->sampling_rate > 0 ? p->sampling_rate : Beeps::sampling_rate();
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
