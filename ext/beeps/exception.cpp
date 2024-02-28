#include "beeps/ruby/exception.h"


#include "defs.h"


static Class cBeepsError;
static Class cOpenALError;

void
Init_beeps_exception ()
{
	Module mBeeps = define_module("Beeps");

	cBeepsError  = mBeeps.define_class("BeepsError", rb_eStandardError);
	cOpenALError = mBeeps.define_class("OpenALError", cBeepsError);
}


namespace Beeps
{


	Class
	beeps_error_class ()
	{
		return cBeepsError;
	}

	Class
	openal_error_class ()
	{
		return cOpenALError;
	}


}// Beeps
