#include "beeps/processor.h"


#include "signalsmith-stretch.h"
#include "signals.h"


namespace Beeps
{


	struct TimeStretch::Data
	{

		float scale = 1;

	};// TimeStretch::Data


	TimeStretch::TimeStretch ()
	{
	}

	TimeStretch::~TimeStretch ()
	{
	}

	void
	TimeStretch::set_scale (float scale)
	{
		self->scale = scale;
	}

	float
	TimeStretch::scale () const
	{
		return self->scale;
	}

	void
	TimeStretch::process (Signals* signals)
	{
		Super::process(signals);

		signalsmith::stretch::SignalsmithStretch<float> stretch;
		stretch.presetDefault(signals->nchannels(), signals->sampling_rate());

		SignalBuffer<float> input(*signals);
		SignalBuffer<float> output(
			signals->nsamples() * scale(), signals->nchannels());

		stretch.process(
			input.channels(),  input.nsamples(),
			output.channels(), output.nsamples());

		Signals_set_buffer(signals, output);
	}

	TimeStretch::operator bool () const
	{
		if (!Super::operator bool()) return false;
		return self->scale > 0;
	}


}// Beeps
