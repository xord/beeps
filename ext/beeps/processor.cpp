#include "beeps/ruby/processor.h"


#include "beeps/exception.h"
#include "defs.h"


RUCY_DEFINE_VALUE_FROM_TO(Beeps::Processor)

#define THIS  to<Beeps::Processor*>(self)

#define CHECK RUCY_CHECK_OBJECT(Beeps::Processor, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	Beeps::beeps_error(__FILE__, __LINE__);
}
RUCY_END


static Class cProcessor;

void
Init_beeps_processor ()
{
	Module mBeeps = define_module("Beeps");

	cProcessor = mBeeps.define_class("Processor");
	cProcessor.define_alloc_func(alloc);
}


namespace Beeps
{


	Class
	processor_class ()
	{
		return cProcessor;
	}


}// Beeps
