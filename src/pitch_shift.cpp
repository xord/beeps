#include "beeps/processor.h"


#include "PitShift.h"
#include "signalsmith-stretch.h"
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

		signalsmith::stretch::SignalsmithStretch<float> stretch;
		stretch.presetDefault(signals->nchannels(), signals->sampling_rate());
		stretch.setTransposeFactor(self->shift);

		SignalBuffer<float> input(*signals);
		SignalBuffer<float> output(signals->nsamples(), signals->nchannels());

		stretch.process(
			input.channels(),  input.nsamples(),
			output.channels(), output.nsamples());

		Signals_set_buffer(signals, output);
	}

	PitchShift::operator bool () const
	{
		if (!Super::operator bool()) return false;
		return self->shift > 0;
	}


}// Beeps
