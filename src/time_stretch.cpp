#include "beeps/filter.h"


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
	:	Super(input)
	{
		set_buffering_seconds(1);
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
		if (scale <= 0)
			argument_error(__FILE__, __LINE__, "'scale' must be greater than 0");

		self->scale = scale;

		set_updated();
	}

	float
	TimeStretch::scale () const
	{
		return self->scale;
	}

	TimeStretch::operator bool () const
	{
		if (!Super::operator bool()) return false;
		return self->scale > 0;
	}

	void
	TimeStretch::filter (Context* context, Signals* signals, uint* offset)
	{
		if (self->scale == 1)
			return Super::filter(context, signals, offset);

		uint nsamples  = signals->capacity();
		Signals source = Signals_create(
			nsamples / self->scale, signals->nchannels(), signals->sample_rate());

		Super::filter(context, &source, offset);

		SignalSamples<float> input(source);
		SignalSamples<float> output(
			source.nsamples() < source.capacity()
				?	source.nsamples() * self->scale
				:	nsamples,
			signals->nchannels());

		self->stretch.presetDefault(signals->nchannels(), signals->sample_rate());
		self->stretch.process(
			 input.channels(),  input.nsamples(),
			output.channels(), output.nsamples());

		Signals_write_samples(signals, output);
	}


}// Beeps
