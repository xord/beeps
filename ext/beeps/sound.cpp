#include "beeps/ruby/sound.h"


#include "defs.h"


RUCY_DEFINE_VALUE_FROM_TO(Beeps::Sound)

#define THIS  to<Beeps::Sound*>(self)

#define CHECK RUCY_CHECK_OBJECT(Beeps::Sound, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return new_type<Beeps::Sound>(klass);
}
RUCY_END

static
RUCY_DEF2(initialize, processor, seconds)
{
	RUCY_CHECK_OBJ(Beeps::Sound, self);

	*THIS = Beeps::Sound(to<Beeps::Processor*>(processor), to<float>(seconds));
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


static Class cSound;

void
Init_beeps_sound ()
{
	Module mBeeps = define_module("Beeps");

	cSound = mBeeps.define_class("Sound");
	cSound.define_alloc_func(alloc);
	cSound.define_private_method("initialize", initialize);
	cSound.define_method("play", play);
}


namespace Beeps
{


	Class
	sound_class ()
	{
		return cSound;
	}


}// Beeps
