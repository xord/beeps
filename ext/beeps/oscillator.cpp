#include "beeps/ruby/processor.h"


#include "defs.h"


RUCY_DEFINE_WRAPPER_VALUE_FROM_TO(Beeps::Oscillator)

#define THIS  to<Beeps::Oscillator*>(self)

#define CHECK RUCY_CHECK_OBJECT(Beeps::Oscillator, self)


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
	return self;
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
RUCY_DEF1(set_frequency, frequency)
{
	CHECK;
	THIS->set_frequency(to<float>(frequency));
	return self;
}
RUCY_END

static
RUCY_DEF0(get_frequency)
{
	CHECK;
	return value(THIS->frequency());
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
	cOscillator.define_method("frequency=", set_frequency);
	cOscillator.define_method("frequency",  get_frequency);
	cOscillator.define_const("NONE",     Beeps::Oscillator::NONE);
	cOscillator.define_const("SINE",     Beeps::Oscillator::SINE);
	cOscillator.define_const("TRIANGLE", Beeps::Oscillator::TRIANGLE);
	cOscillator.define_const("SQUARE",   Beeps::Oscillator::SQUARE);
	cOscillator.define_const("SAWTOOTH", Beeps::Oscillator::SAWTOOTH);
}


namespace Beeps
{


	Class
	oscillator_class ()
	{
		return cOscillator;
	}


}// Beeps
