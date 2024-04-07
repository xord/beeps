#include "beeps/ruby/filter.h"


#include "beeps/ruby/processor.h"
#include "defs.h"


RUCY_DEFINE_WRAPPER_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::Gain)

#define THIS  to<Beeps::Gain*>(self)

#define CHECK RUCY_CHECK_OBJ(Beeps::Gain, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return value(new Beeps::RubyProcessor<Beeps::Gain>, klass);
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


static Class cGain;

void
Init_beeps_gain ()
{
	Module mBeeps = define_module("Beeps");

	cGain = mBeeps.define_class("Gain", Beeps::processor_class());
	cGain.define_alloc_func(alloc);
	cGain.define_method("gain=", set_gain);
	cGain.define_method("gain",  get_gain);
}


namespace Beeps
{


	Class
	gain_class ()
	{
		return cGain;
	}


}// Beeps
