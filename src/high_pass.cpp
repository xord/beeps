#include "x_pass.h"


namespace Beeps
{


	struct HighPass::Data : public xPassFilterData
	{

		Data ()
		:	xPassFilterData(100)
		{
		}

	};// HighPass::Data


	HighPass::HighPass (Processor* input)
	:	Super(input)
	{
	}

	HighPass::~HighPass ()
	{
	}

	void
	HighPass::set_cutoff_frequency (float frequency)
	{
		if (frequency == self->cutoff_freq)
			return;

		self->cutoff_freq = frequency;
		set_updated();
	}

	float
	HighPass::cutoff_frequency () const
	{
		return self->cutoff_freq;
	}

	void
	HighPass::filter (Context* context, Signals* signals, uint* offset)
	{
		Super::filter(context, signals, offset);

		self->biquad.setHighPass(self->cutoff_freq);
		self->tick(signals);
	}


}// Beeps
