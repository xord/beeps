#include "beeps/processor.h"


#include "PitShift.h"
#include "signals.h"


namespace Beeps
{


	struct PitchShift::Data
	{

		float shift = 1;

	};// PitchShift::Data


	PitchShift::PitchShift ()
	{
	}

	PitchShift::~PitchShift ()
	{
	}

	void
	PitchShift::set_shift (float shift)
	{
		self->shift = shift;
	}

	float
	PitchShift::shift () const
	{
		return self->shift;
	}

	void
	PitchShift::process (Signals* signals)
	{
		Super::process(signals);

		stk::StkFrames* f = Signals_get_frames(signals);
		if (!f)
			invalid_state_error(__FILE__, __LINE__);

		stk::PitShift shift;
		shift.setShift(self->shift);

		for (uint ch = 0; ch < signals->nchannels(); ++ch)
			shift.tick(*f, ch);
	}

	PitchShift::operator bool () const
	{
		return self->shift > 0;
	}


}// Beeps
