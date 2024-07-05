#include "beeps/ruby/filter.h"


#include "beeps/ruby/processor.h"
#include "defs.h"


RUCY_DEFINE_WRAPPER_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::PitchShift)

#define THIS  to<Beeps::PitchShift*>(self)

#define CHECK RUCY_CHECK_OBJ(Beeps::PitchShift, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return value(new Beeps::RubyProcessor<Beeps::PitchShift>, klass);
}
RUCY_END

static
RUCY_DEF1(set_shift, shift)
{
	CHECK;

	THIS->set_shift(to<float>(shift));
	return shift;
}
RUCY_END

static
RUCY_DEF0(get_shift)
{
	CHECK;

	return value(THIS->shift());
}
RUCY_END


static Class cPitchShift;

void
Init_beeps_pitch_shift ()
{
	Module mBeeps = define_module("Beeps");

	cPitchShift = mBeeps.define_class("PitchShift", Beeps::processor_class());
	cPitchShift.define_alloc_func(alloc);
	cPitchShift.define_method("shift=", set_shift);
	cPitchShift.define_method("shift",  get_shift);
}


namespace Beeps
{


	Class
	pitch_shift_class ()
	{
		return cPitchShift;
	}


}// Beeps
