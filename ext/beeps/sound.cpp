#include "beeps/ruby/sound.h"


#include "defs.h"


RUCY_DEFINE_VALUE_FROM_TO(Beeps::Sound)

#define THIS  to<Beeps::Sound*>(self)

#define CHECK RUCY_CHECK_OBJ(Beeps::Sound, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return new_type<Beeps::Sound>(klass);
}
RUCY_END

static
RUCY_DEF4(setup, processor, seconds, nchannels, sample_rate)
{
	CHECK;

	Beeps::Processor* proc = to<Beeps::Processor*>(processor);
	float secs             = seconds ? to<float>(seconds) : 0;

	if (!nchannels && !sample_rate)
		*THIS = Beeps::Sound(proc, secs);
	else if (nchannels && !sample_rate)
		*THIS = Beeps::Sound(proc, secs, to<uint>(nchannels));
	else
		*THIS = Beeps::Sound(proc, secs, to<uint>(nchannels), to<double>(sample_rate));

	return self;
}
RUCY_END

static
RUCY_DEF0(play)
{
	CHECK;

	THIS->play();
	return self;
}
RUCY_END

static
RUCY_DEF1(save, path)
{
	CHECK;

	THIS->save(path.c_str());
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

static
RUCY_DEF0(get_seconds)
{
	CHECK;

	return value(THIS->seconds());
}
RUCY_END


static Class cSound;

void
Init_beeps_sound ()
{
	Module mBeeps = define_module("Beeps");

	cSound = mBeeps.define_class("Sound");
	cSound.define_alloc_func(alloc);
	cSound.define_private_method("setup", setup);
	cSound.define_method("play", play);
	cSound.define_method("save", save);
	cSound.define_method("sample_rate", get_sample_rate);
	cSound.define_method("nchannels",   get_nchannels);
	cSound.define_method("seconds",     get_seconds);
}


namespace Beeps
{


	Class
	sound_class ()
	{
		return cSound;
	}


}// Beeps
