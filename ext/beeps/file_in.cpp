#include "beeps/ruby/processor.h"


#include "defs.h"


RUCY_DEFINE_WRAPPER_VALUE_FROM_TO(Beeps::FileIn)

#define THIS  to<Beeps::FileIn*>(self)

#define CHECK RUCY_CHECK_OBJ(Beeps::FileIn, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return value(new Beeps::RubyProcessor<Beeps::FileIn>, klass);
}
RUCY_END

static
RUCY_DEF1(set_path, path)
{
	CHECK;
	THIS->set_path(to<const char*>(path));
	return self;
}
RUCY_END

static
RUCY_DEF0(get_path)
{
	CHECK;
	return value(THIS->path());
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

static
RUCY_DEF0(get_seconds)
{
	CHECK;
	return value(THIS->seconds());
}
RUCY_END


static Class cFileIn;

void
Init_beeps_file_in ()
{
	Module mBeeps = define_module("Beeps");

	cFileIn = mBeeps.define_class("FileIn", Beeps::processor_class());
	cFileIn.define_alloc_func(alloc);
	cFileIn.define_method("path=",       set_path);
	cFileIn.define_method("path",        get_path);
	cFileIn.define_method("sample_rate", get_sample_rate);
	cFileIn.define_method("nchannels",   get_nchannels);
	cFileIn.define_method("seconds",     get_seconds);
}


namespace Beeps
{


	Class
	file_in_class ()
	{
		return cFileIn;
	}


}// Beeps
