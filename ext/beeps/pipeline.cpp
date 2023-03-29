#include "beeps/ruby/processor.h"


#include "defs.h"


RUCY_DEFINE_WRAPPER_VALUE_FROM_TO(Beeps::Pipeline)

#define THIS  to<Beeps::Pipeline*>(self)

#define CHECK RUCY_CHECK_OBJECT(Beeps::Pipeline, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return value(new Beeps::RubyProcessor<Beeps::Pipeline>, klass);
}
RUCY_END

static
RUCY_DEF1(add, processor)
{
	CHECK;
	THIS->add(to<Beeps::Processor*>(processor));
	return self;
}
RUCY_END


static Class cPipeline;

void
Init_beeps_pipeline ()
{
	Module mBeeps = define_module("Beeps");

	cPipeline = mBeeps.define_class("Pipeline", Beeps::processor_class());
	cPipeline.define_alloc_func(alloc);
	cPipeline.define_private_method("add!", add);
}


namespace Beeps
{


	Class
	pipeline_class ()
	{
		return cPipeline;
	}


}// Beeps
