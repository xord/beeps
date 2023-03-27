#include "beeps/ruby/processor.h"


#include "defs.h"


RUCY_DEFINE_VALUE_FROM_TO(Beeps::Oscillator)

#define THIS  to<Beeps::Oscillator*>(self)

#define CHECK RUCY_CHECK_OBJECT(Beeps::Oscillator, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return new_type<Beeps::Oscillator>(klass);
}
RUCY_END

static
RUCY_DEF1(set_frequency, frequency)
{
	CHECK;
	THIS->set_frequency(frequency.as_f(true));
	return self;
}
RUCY_END

static
RUCY_DEF0(frequency)
{
	CHECK;
	return to<float>(THIS->frequency());
}
RUCY_END


static Class cOscillator;

void
Init_beeps_oscillator ()
{
	Module mBeeps = define_module("Beeps");

	cOscillator = mBeeps.define_class("Oscillator", Beeps::processor_class());
	cOscillator.define_alloc_func(alloc);
	cOscillator.define_method("frequency=", set_frequency);
	cOscillator.define_method("frequency",      frequency);
}


namespace Beeps
{


	Class
	oscillator_class ()
	{
		return cOscillator;
	}


}// Beeps
