#include "beeps/filter.h"


#include "signals.h"


namespace Beeps
{


	struct Gain::Data
	{

		float gain = 1;

	};// Gain::Data


	Gain::Gain (Processor* input)
	:	Super(input)
	{
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

	bool
	Gain::seekable () const
	{
		const Processor* in = input();
		return !in || in->seekable();
	}

	void
	Gain::filter (Context* context, Signals* signals, uint* offset)
	{
		Super::filter(context, signals, offset);

		float gain = self->gain;
		uint size  = signals->nsamples() * signals->nchannels();
		Sample* p  = Signals_at(signals, 0);
		for (uint i = 0; i < size; ++i)
			*p++ *= gain;
	}


}// Beeps
