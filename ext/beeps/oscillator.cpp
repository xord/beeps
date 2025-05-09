#include "beeps/ruby/generator.h"


#include "beeps/ruby/processor.h"
#include "defs.h"


RUCY_DEFINE_WRAPPER_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::Oscillator)

#define THIS  to<Beeps::Oscillator*>(self)

#define CHECK RUCY_CHECK_OBJ(Beeps::Oscillator, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return value(new Beeps::RubyProcessor<Beeps::Oscillator>, klass);
}
RUCY_END

static
RUCY_DEF1(set_type, type)
{
	CHECK;

	THIS->set_type((Beeps::Oscillator::Type) to<uint>(type));
	return type;
}
RUCY_END

static
RUCY_DEF0(get_type)
{
	CHECK;

	return value(THIS->type());
}
RUCY_END

static
RUCY_DEF1(set_samples, samples)
{
	CHECK;

	Value* array = samples.as_array();
	size_t size  = samples.size();

	std::vector<Beeps::Sample> data;
	data.reserve(size);
	for (size_t i = 0; i < size; ++i)
		data.push_back(to<Beeps::Sample>(array[i]));

	THIS->set_samples(&data[0], data.size());
}
RUCY_END

static
RUCY_DEF0(each_sample)
{
	CHECK;

	const Beeps::Sample* samples = THIS->samples();
	size_t size                  = THIS->nsamples();
	if (!samples || size == 0) return Qnil;

	Value ret;
	for (size_t i = 0; i < size; ++i)
		ret = rb_yield(value((float) samples[i]));
	return ret;
}
RUCY_END

static
RUCY_DEF1(set_frequency, frequency)
{
	CHECK;

	if (frequency.is_a(Beeps::processor_class()))
		THIS->set_frequency(to<Beeps::Processor*>(frequency));
	else
		THIS->set_frequency(to<float>(frequency));

	return frequency;
}
RUCY_END

static
RUCY_DEF0(get_frequency)
{
	CHECK;

	return value(THIS->frequency());
}
RUCY_END

static
RUCY_DEF1(set_phase, phase)
{
	CHECK;

	THIS->set_phase(to<float>(phase));
	return phase;
}
RUCY_END

static
RUCY_DEF0(get_phase)
{
	CHECK;

	return value(THIS->phase());
}
RUCY_END

static
RUCY_DEF1(set_gain, gain)
{
	CHECK;

	THIS->set_gain(to<float>(gain));
	return gain;
}
RUCY_END

static
RUCY_DEF0(get_gain)
{
	CHECK;

	return value(THIS->gain());
}
RUCY_END

static
RUCY_DEF1(set_offset, offset)
{
	CHECK;

	THIS->set_offset(to<float>(offset));
	return offset;
}
RUCY_END

static
RUCY_DEF0(get_offset)
{
	CHECK;

	return value(THIS->offset());
}
RUCY_END

static
RUCY_DEF1(set_duty, duty)
{
	CHECK;

	THIS->set_duty(to<float>(duty));
	return duty;
}
RUCY_END

static
RUCY_DEF0(get_duty)
{
	CHECK;

	return value(THIS->duty());
}
RUCY_END


static Class cOscillator;

void
Init_beeps_oscillator ()
{
	Module mBeeps = define_module("Beeps");

	cOscillator = mBeeps.define_class("Oscillator", Beeps::processor_class());
	cOscillator.define_alloc_func(alloc);
	cOscillator.define_method("type=", set_type);
	cOscillator.define_method("type",  get_type);
	cOscillator.define_method(     "samples=", set_samples);
	cOscillator.define_method("each_sample!", each_sample);
	cOscillator.define_method("frequency=", set_frequency);
	cOscillator.define_method("frequency",  get_frequency);
	cOscillator.define_method("phase=",     set_phase);
	cOscillator.define_method("phase",      get_phase);
	cOscillator.define_method("gain=",      set_gain);
	cOscillator.define_method("gain",       get_gain);
	cOscillator.define_method("offset=",    set_offset);
	cOscillator.define_method("offset",     get_offset);
	cOscillator.define_method("duty=",      set_duty);
	cOscillator.define_method("duty",       get_duty);

	cOscillator.define_const("TYPE_NONE", Beeps::Oscillator::TYPE_NONE);
	cOscillator.define_const("SINE",      Beeps::Oscillator::SINE);
	cOscillator.define_const("TRIANGLE",  Beeps::Oscillator::TRIANGLE);
	cOscillator.define_const("SQUARE",    Beeps::Oscillator::SQUARE);
	cOscillator.define_const("SAWTOOTH",  Beeps::Oscillator::SAWTOOTH);
	cOscillator.define_const("NOISE",     Beeps::Oscillator::NOISE);
	cOscillator.define_const("SAMPLES",   Beeps::Oscillator::SAMPLES);
}


namespace Beeps
{


	Class
	oscillator_class ()
	{
		return cOscillator;
	}


}// Beeps
