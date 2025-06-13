#include "beeps/ruby/sound.h"


#include "defs.h"


RUCY_DEFINE_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::Sound)

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

	*THIS = Beeps::Sound(
		to<Beeps::Processor*>(processor),
		to<float>(seconds), to<uint>(nchannels), to<double>(sample_rate));
	return self;
}
RUCY_END

static
RUCY_DEF0(play)
{
	CHECK;

	return value(THIS->play());
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

static
RUCY_DEF1(set_gain, gain)
{
	CHECK;

	THIS->set_gain(to<float>(gain));
	return gain;
}
RUCY_END

static
RUCY_DEF0(get_gain)
{
	CHECK;

	return value(THIS->gain());
}
RUCY_END

static
RUCY_DEF1(set_loop, loop)
{
	CHECK;

	THIS->set_loop(to<bool>(loop));
	return loop;
}
RUCY_END

static
RUCY_DEF0(get_loop)
{
	CHECK;

	return value(THIS->loop());
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
RUCY_DEF1(load, path)
{
	return value(Beeps::load_sound(path.c_str()));
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
	cSound.define_private_method("play!", play);
	cSound.define_method("sample_rate", get_sample_rate);
	cSound.define_method("nchannels",   get_nchannels);
	cSound.define_method("seconds",     get_seconds);
	cSound.define_method("gain=", set_gain);
	cSound.define_method("gain",  get_gain);
	cSound.define_method("loop=", set_loop);
	cSound.define_method("loop",  get_loop);
	cSound.define_method("save", save);
	cSound.define_singleton_method("load", load);
}


namespace Beeps
{


	Class
	sound_class ()
	{
		return cSound;
	}


}// Beeps
