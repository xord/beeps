#include "signals.h"


#include <memory>
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


	Signals::Signals (float seconds, uint channels)
	{
		if (seconds <= 0 || channels <= 0)
			return;

		self->frames.reset(new stk::StkFrames(seconds * sampling_rate(), channels));
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

	float
	Signals::seconds () const
	{
		double sec = samples() / (double) sampling_rate();
		return (float) sec;
	}

	uint
	Signals::samples () const
	{
		Data* p = self.get();
		return p ? p->frames->frames() : 0;
	}

	uint
	Signals::channels () const
	{
		Data* p = self.get();
		return p ? p->frames->channels() : 0;
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
