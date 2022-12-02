#include "beeps/ruby/processor.h"


#include "defs.h"


RUCY_DEFINE_VALUE_FROM_TO(Beeps::SquareWave)

#define THIS  to<Beeps::SquareWave*>(self)

#define CHECK RUCY_CHECK_OBJECT(Beeps::SquareWave, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return new_type<Beeps::SquareWave>(klass);
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


static Class cSquareWave;

void
Init_beeps_square_wave ()
{
	Module mBeeps = define_module("Beeps");

	cSquareWave = mBeeps.define_class("SquareWave", Beeps::processor_class());
	cSquareWave.define_alloc_func(alloc);
	cSquareWave.define_method("frequency=", set_frequency);
	cSquareWave.define_method("frequency",      frequency);
}


namespace Beeps
{


	Class
	square_wave_class ()
	{
		return cSquareWave;
	}


}// Beeps
