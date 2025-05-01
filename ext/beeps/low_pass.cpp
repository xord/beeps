#include "beeps/ruby/filter.h"


#include "beeps/ruby/processor.h"
#include "defs.h"


RUCY_DEFINE_WRAPPER_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::LowPass)

#define THIS  to<Beeps::LowPass*>(self)

#define CHECK RUCY_CHECK_OBJ(Beeps::LowPass, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return value(new Beeps::RubyProcessor<Beeps::LowPass>, klass);
}
RUCY_END

static
RUCY_DEF1(set_cutoff_frequency, frequency)
{
	CHECK;

	THIS->set_cutoff_frequency(to<float>(frequency));
}
RUCY_END

static
RUCY_DEF0(get_cutoff_frequency)
{
	CHECK;

	return value(THIS->cutoff_frequency());
}
RUCY_END


static Class cLowPass;

void
Init_beeps_low_pass ()
{
	Module mBeeps = define_module("Beeps");

	cLowPass = mBeeps.define_class("LowPass", Beeps::processor_class());
	cLowPass.define_alloc_func(alloc);
	cLowPass.define_method("cutoff_frequency=", set_cutoff_frequency);
	cLowPass.define_method("cutoff_frequency",  get_cutoff_frequency);
}


namespace Beeps
{


	Class
	low_pass_class ()
	{
		return cLowPass;
	}


}// Beeps
