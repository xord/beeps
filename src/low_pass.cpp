#include "x_pass.h"


namespace Beeps
{


	struct LowPass::Data : public xPassFilterData
	{

		Data ()
		:	xPassFilterData(1000)
		{
		}

	};// LowPass::Data


	LowPass::LowPass (Processor* input)
	:	Super(input)
	{
	}

	LowPass::~LowPass ()
	{
	}

	void
	LowPass::set_cutoff_frequency (float frequency)
	{
		if (frequency <= 0)
			argument_error(__FILE__, __LINE__);

		if (frequency == self->cutoff_freq)
			return;

		self->cutoff_freq = frequency;
		set_updated();
	}

	float
	LowPass::cutoff_frequency () const
	{
		return self->cutoff_freq;
	}

	void
	LowPass::filter (Context* context, Signals* signals, uint* offset)
	{
		Super::filter(context, signals, offset);

		self->biquad.setLowPass(self->cutoff_freq);
		self->tick(signals);
	}


}// Beeps
