#include "beeps/ruby/filter.h"


#include "beeps/ruby/processor.h"
#include "defs.h"


RUCY_DEFINE_WRAPPER_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::Mixer)

#define THIS  to<Beeps::Mixer*>(self)

#define CHECK RUCY_CHECK_OBJ(Beeps::Mixer, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return value(new Beeps::RubyProcessor<Beeps::Mixer>, klass);
}
RUCY_END

static
RUCY_DEF1(add_input, inputs)
{
	CHECK;

	const Value* array = inputs.as_array();
	size_t size        = inputs.size();

	for (size_t i = 0; i < size; ++i)
		THIS->add_input(to<Beeps::Processor*>(array[i]));
	return inputs;
}
RUCY_END

static
RUCY_DEF1(remove_input, inputs)
{
	CHECK;

	const Value* array = inputs.as_array();
	size_t size        = inputs.size();

	for (size_t i = 0; i < size; ++i)
		THIS->remove_input(to<Beeps::Processor*>(array[i]));
	return inputs;
}
RUCY_END

static
RUCY_DEF0(each_input)
{
	CHECK;

	Value ret;
	for (auto& input : *THIS)
		ret = rb_yield(value(input.get()));
	return ret;
}
RUCY_END


static Class cMixer;

void
Init_beeps_mixer ()
{
	Module mBeeps = define_module("Beeps");

	cMixer = mBeeps.define_class("Mixer", Beeps::processor_class());
	cMixer.define_alloc_func(alloc);
	cMixer.define_method(   "add_input!",    add_input);
	cMixer.define_method("remove_input!", remove_input);
	cMixer.define_method(  "each_input!",   each_input);
}


namespace Beeps
{


	Class
	mixer_class ()
	{
		return cMixer;
	}


}// Beeps
