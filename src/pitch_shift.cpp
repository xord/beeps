#include "beeps/filter.h"


#include "PitShift.h"
#include "signalsmith-stretch.h"
#include "signals.h"


namespace Beeps
{


	struct PitchShift::Data
	{

		signalsmith::stretch::SignalsmithStretch<float> stretch;

		float shift = 1;

	};// PitchShift::Data


	PitchShift::PitchShift (Processor* input)
	:	Super(1)
	{
		set_input(input);
	}

	PitchShift::~PitchShift ()
	{
	}

	void
	PitchShift::reset ()
	{
		Super::reset();

		self->stretch.reset();
	}

	void
	PitchShift::set_shift (float shift)
	{
		self->shift = shift;

		set_updated();
	}

	float
	PitchShift::shift () const
	{
		return self->shift;
	}

	void
	PitchShift::filter (Context* context, Signals* signals, uint* offset)
	{
		Super::filter(context, signals, offset);

		if (self->shift == 1 || signals->nsamples() <= 0)
			return;

		SignalSamples<float> input(*signals);
		SignalSamples<float> output(input.nsamples(), signals->nchannels());

		self->stretch.presetDefault(signals->nchannels(), signals->sample_rate());
		self->stretch.setTransposeFactor(self->shift);
		self->stretch.process(
			 input.channels(),  input.nsamples(),
			output.channels(), output.nsamples());

		Signals_write_samples(signals, output, signals->nsamples());
	}

	PitchShift::operator bool () const
	{
		if (!Super::operator bool()) return false;
		return self->shift > 0;
	}


}// Beeps
