#include "beeps/ruby/filter.h"


#include "beeps/ruby/processor.h"
#include "defs.h"


RUCY_DEFINE_WRAPPER_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::TimeStretch)

#define THIS  to<Beeps::TimeStretch*>(self)

#define CHECK RUCY_CHECK_OBJ(Beeps::TimeStretch, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return value(new Beeps::RubyProcessor<Beeps::TimeStretch>, klass);
}
RUCY_END

static
RUCY_DEF1(set_scale, scale)
{
	CHECK;

	THIS->set_scale(to<float>(scale));
	return scale;
}
RUCY_END

static
RUCY_DEF0(get_scale)
{
	CHECK;

	return value(THIS->scale());
}
RUCY_END


static Class cTimeStretch;

void
Init_beeps_time_stretch ()
{
	Module mBeeps = define_module("Beeps");

	cTimeStretch = mBeeps.define_class("TimeStretch", Beeps::processor_class());
	cTimeStretch.define_alloc_func(alloc);
	cTimeStretch.define_method("scale=", set_scale);
	cTimeStretch.define_method("scale",  get_scale);
}


namespace Beeps
{


	Class
	time_stretch_class ()
	{
		return cTimeStretch;
	}


}// Beeps
