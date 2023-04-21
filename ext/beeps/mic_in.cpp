#include "beeps/ruby/generator.h"


#include "beeps/ruby/processor.h"
#include "defs.h"


RUCY_DEFINE_WRAPPER_VALUE_FROM_TO(Beeps::MicIn)

#define THIS  to<Beeps::MicIn*>(self)

#define CHECK RUCY_CHECK_OBJ(Beeps::MicIn, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return value(new Beeps::RubyProcessor<Beeps::MicIn>, klass);
}
RUCY_END

static
RUCY_DEF0(start)
{
	CHECK;

	THIS->start();
	return self;
}
RUCY_END

static
RUCY_DEF0(stop)
{
	CHECK;

	THIS->stop();
	return self;
}
RUCY_END

static
RUCY_DEF0(get_sample_rate)
{
	CHECK;

	return value(THIS->sample_rate());
}
RUCY_END

static
RUCY_DEF0(get_nchannels)
{
	CHECK;

	return value(THIS->nchannels());
}
RUCY_END


static Class cMicIn;

void
Init_beeps_mic_in ()
{
	Module mBeeps = define_module("Beeps");

	cMicIn = mBeeps.define_class("MicIn", Beeps::processor_class());
	cMicIn.define_alloc_func(alloc);
	cMicIn.define_method("start", start);
	cMicIn.define_method("stop",  stop);
	cMicIn.define_method("sample_rate", get_sample_rate);
	cMicIn.define_method("nchannels",   get_nchannels);
}


namespace Beeps
{


	Class
	mic_in_class ()
	{
		return cMicIn;
	}


}// Beeps
