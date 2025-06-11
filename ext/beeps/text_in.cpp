#include "beeps/ruby/generator.h"


#include "beeps/ruby/processor.h"
#include "defs.h"


RUCY_DEFINE_WRAPPER_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::TextIn)

#define THIS  to<Beeps::TextIn*>(self)

#define CHECK RUCY_CHECK_OBJ(Beeps::TextIn, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return value(new Beeps::RubyProcessor<Beeps::TextIn>, klass);
}
RUCY_END

static
RUCY_DEF0(get_sample_rate)
{
	CHECK;

	return value(THIS->sample_rate());
}
RUCY_END


static Class cTextIn;

void
Init_beeps_text_in ()
{
	Module mBeeps = define_module("Beeps");

	cTextIn = mBeeps.define_class("TextIn", Beeps::processor_class());
	cTextIn.define_alloc_func(alloc);
	cTextIn.define_method("sample_rate", get_sample_rate);
}


namespace Beeps
{


	Class
	text_in_class ()
	{
		return cTextIn;
	}


}// Beeps
