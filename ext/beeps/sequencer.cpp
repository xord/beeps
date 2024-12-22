#include "beeps/ruby/generator.h"


#include "beeps/ruby/processor.h"
#include "defs.h"


RUCY_DEFINE_WRAPPER_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::Sequencer)

#define THIS  to<Beeps::Sequencer*>(self)

#define CHECK RUCY_CHECK_OBJ(Beeps::Sequencer, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return value(new Beeps::RubyProcessor<Beeps::Sequencer>, klass);
}
RUCY_END

static
RUCY_DEF3(add, processor, offset, duration)
{
	CHECK;

	THIS->add(
		to<Beeps::Processor*>(processor),
		to<float>(offset),
		to<float>(duration));
}
RUCY_END

static
RUCY_DEF2(remove, processor, offset)
{
	CHECK;

	THIS->remove(
		to<Beeps::Processor*>(processor),
		to<float>(offset));
}
RUCY_END

static
RUCY_DEF1(set_time_scale, time_scale)
{
	CHECK;

	THIS->set_time_scale(to<float>(time_scale));
	return time_scale;
}
RUCY_END

static
RUCY_DEF0(get_time_scale)
{
	CHECK;

	return value(THIS->time_scale());
}
RUCY_END


static Class cSequencer;

void
Init_beeps_sequencer ()
{
	Module mBeeps = define_module("Beeps");

	cSequencer = mBeeps.define_class("Sequencer", Beeps::processor_class());
	cSequencer.define_alloc_func(alloc);
	cSequencer.define_method("add",    add);
	cSequencer.define_method("remove", remove);
	//cSequencer.define_method("time_scale=", set_time_scale);
	//cSequencer.define_method("time_scale",  get_time_scale);
}


namespace Beeps
{


	Class
	sequencer_class ()
	{
		return cSequencer;
	}


}// Beeps
