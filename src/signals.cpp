#include "beeps/signals.h"


#include <boost/scoped_ptr.hpp>
#include "Stk.h"
#include "beeps/beeps.h"
#include "beeps/exception.h"


namespace Beeps
{


	struct Signals::Data
	{

		boost::scoped_ptr<stk::StkFrames> frames;

	};// Signals::Data


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
	Signals::copy () const
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

	stk::StkFrames*
	Signals::frames ()
	{
		Data* p = self.get();
		return p ? p->frames.get() : NULL;
	}

	const stk::StkFrames*
	Signals::frames () const
	{
		return const_cast<Signals*>(this)->frames();
	}

	Signals::operator bool () const
	{
		const stk::StkFrames* f = frames();
		return f && f->frames() > 0 && f->channels() > 0;
	}

	bool
	Signals::operator ! () const
	{
		return !operator bool();
	}


}// Beeps
