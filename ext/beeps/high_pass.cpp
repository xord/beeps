#include "beeps/ruby/filter.h"


#include "beeps/ruby/processor.h"
#include "defs.h"


RUCY_DEFINE_WRAPPER_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::HighPass)

#define THIS  to<Beeps::HighPass*>(self)

#define CHECK RUCY_CHECK_OBJ(Beeps::HighPass, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return value(new Beeps::RubyProcessor<Beeps::HighPass>, klass);
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


static Class cHighPass;

void
Init_beeps_high_pass ()
{
	Module mBeeps = define_module("Beeps");

	cHighPass = mBeeps.define_class("HighPass", Beeps::processor_class());
	cHighPass.define_alloc_func(alloc);
	cHighPass.define_method("cutoff_frequency=", set_cutoff_frequency);
	cHighPass.define_method("cutoff_frequency",  get_cutoff_frequency);
}


namespace Beeps
{


	Class
	high_pass_class ()
	{
		return cHighPass;
	}


}// Beeps
