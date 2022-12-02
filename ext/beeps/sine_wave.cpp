#include "beeps/ruby/processor.h"


#include "defs.h"


RUCY_DEFINE_VALUE_FROM_TO(Beeps::SineWave)

#define THIS  to<Beeps::SineWave*>(self)

#define CHECK RUCY_CHECK_OBJECT(Beeps::SineWave, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return new_type<Beeps::SineWave>(klass);
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


static Class cSineWave;

void
Init_beeps_sine_wave ()
{
	Module mBeeps = define_module("Beeps");

	cSineWave = mBeeps.define_class("SineWave", Beeps::processor_class());
	cSineWave.define_alloc_func(alloc);
	cSineWave.define_method("frequency=", set_frequency);
	cSineWave.define_method("frequency",      frequency);
}


namespace Beeps
{


	Class
	sine_wave_class ()
	{
		return cSineWave;
	}


}// Beeps
