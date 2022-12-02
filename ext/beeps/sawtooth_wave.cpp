#include "beeps/ruby/processor.h"


#include "defs.h"


RUCY_DEFINE_VALUE_FROM_TO(Beeps::SawtoothWave)

#define THIS  to<Beeps::SawtoothWave*>(self)

#define CHECK RUCY_CHECK_OBJECT(Beeps::SawtoothWave, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return new_type<Beeps::SawtoothWave>(klass);
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


static Class cSawtoothWave;

void
Init_beeps_sawtooth_wave ()
{
	Module mBeeps = define_module("Beeps");

	cSawtoothWave = mBeeps.define_class("SawtoothWave", Beeps::processor_class());
	cSawtoothWave.define_alloc_func(alloc);
	cSawtoothWave.define_method("frequency=", set_frequency);
	cSawtoothWave.define_method("frequency",      frequency);
}


namespace Beeps
{


	Class
	sawtooth_wave_class ()
	{
		return cSawtoothWave;
	}


}// Beeps
