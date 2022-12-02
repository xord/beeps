#include "beeps/ruby/processor.h"


#include "defs.h"


RUCY_DEFINE_VALUE_FROM_TO(Beeps::FileIn)

#define THIS  to<Beeps::FileIn*>(self)

#define CHECK RUCY_CHECK_OBJECT(Beeps::FileIn, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return new_type<Beeps::FileIn>(klass);
}
RUCY_END

static
RUCY_DEF1(initialize, path)
{
	RUCY_CHECK_OBJ(Beeps::FileIn, self);

	*THIS = Beeps::FileIn(to<const char*>(path));
	return self;
}
RUCY_END


static Class cFileIn;

void
Init_beeps_file_in ()
{
	Module mBeeps = define_module("Beeps");

	cFileIn = mBeeps.define_class("FileIn", Beeps::processor_class());
	cFileIn.define_alloc_func(alloc);
	cFileIn.define_private_method("initialize", initialize);
}


namespace Beeps
{


	Class
	file_in_class ()
	{
		return cFileIn;
	}


}// Beeps
