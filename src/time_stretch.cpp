#include "beeps/processor.h"


#include "signalsmith-stretch.h"
#include "signals.h"


namespace Beeps
{


	struct TimeStretch::Data
	{

		signalsmith::stretch::SignalsmithStretch<float> stretch;

		float scale = 1;

	};// TimeStretch::Data


	TimeStretch::TimeStretch (Processor* input)
	{
		set_input(input);
	}

	TimeStretch::~TimeStretch ()
	{
	}

	void
	TimeStretch::reset ()
	{
		Super::reset();

		self->stretch.reset();
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

		if (self->scale == 1) return;

		SignalBuffer<float> input(*signals);
		SignalBuffer<float> output(
			signals->nsamples() * scale(), signals->nchannels());

		self->stretch.presetDefault(signals->nchannels(), signals->sample_rate());
		self->stretch.process(
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
