#include "beeps/filter.h"


#include "signals.h"


namespace Beeps
{


	struct Gain::Data
	{

		float gain = 1;

	};// Gain::Data


	Gain::Gain (Processor* input)
	{
		set_input(input);
	}

	Gain::~Gain ()
	{
	}

	void
	Gain::set_gain (float gain)
	{
		self->gain = gain;

		set_updated();
	}

	float
	Gain::gain () const
	{
		return self->gain;
	}

	void
	Gain::filter (Context* context, Signals* signals, uint* offset)
	{
		Super::filter(context, signals, offset);

		Frames* frames = Signals_get_frames(signals);
		if (!frames)
			argument_error(__FILE__, __LINE__);

		*frames *= self->gain;
	}


}// Beeps
