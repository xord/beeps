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
RUCY_DEF1(set_frequency, frequency)
{
	CHECK;

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
	cOscillator.define_method("phase=", set_phase);
	cOscillator.define_method("phase",  get_phase);

	cOscillator.define_const("TYPE_NONE", Beeps::Oscillator::TYPE_NONE);
	cOscillator.define_const("SINE",      Beeps::Oscillator::SINE);
	cOscillator.define_const("TRIANGLE",  Beeps::Oscillator::TRIANGLE);
	cOscillator.define_const("SQUARE",    Beeps::Oscillator::SQUARE);
	cOscillator.define_const("SAWTOOTH",  Beeps::Oscillator::SAWTOOTH);
}


namespace Beeps
{


	Class
	oscillator_class ()
	{
		return cOscillator;
	}


}// Beeps
