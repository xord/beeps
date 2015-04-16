#include "beeps/ruby/processor.h"


#include <rucy.h>
#include "beeps/exception.h"
#include "defs.h"


using namespace Rucy;


RUCY_DEFINE_VALUE_FROM_TO(Beeps::FileInput)

#define THIS  to<Beeps::FileInput*>(self)

#define CHECK RUCY_CHECK_OBJECT(Beeps::FileInput, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return new_type<Beeps::FileInput>(klass);
}
RUCY_END

static
RUCY_DEF1(initialize, path)
{
	RUCY_CHECK_OBJ(Beeps::FileInput, self);

	*THIS = Beeps::FileInput(to<const char*>(path));
	return self;
}
RUCY_END


static Class cFileInput;

void
Init_file_input ()
{
	Module mBeeps = define_module("Beeps");

	cFileInput = mBeeps.define_class("FileInput", Beeps::processor_class());
	cFileInput.define_alloc_func(alloc);
	cFileInput.define_private_method("initialize", initialize);
}


namespace Beeps
{


	Class
	file_input_class ()
	{
		return cFileInput;
	}


}// Beeps
