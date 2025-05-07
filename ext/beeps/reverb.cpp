#include "beeps/ruby/filter.h"


#include "beeps/ruby/processor.h"
#include "defs.h"


RUCY_DEFINE_WRAPPER_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::Reverb)

#define THIS  to<Beeps::Reverb*>(self)

#define CHECK RUCY_CHECK_OBJ(Beeps::Reverb, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return value(new Beeps::RubyProcessor<Beeps::Reverb>, klass);
}
RUCY_END

static
RUCY_DEF1(set_mix, mix)
{
	CHECK;

	THIS->set_mix(to<float>(mix));
}
RUCY_END

static
RUCY_DEF0(get_mix)
{
	CHECK;

	return value(THIS->mix());
}
RUCY_END

static
RUCY_DEF1(set_room_size, size)
{
	CHECK;

	THIS->set_room_size(to<float>(size));
}
RUCY_END

static
RUCY_DEF0(get_room_size)
{
	CHECK;

	return value(THIS->room_size());
}
RUCY_END

static
RUCY_DEF1(set_damping, damping)
{
	CHECK;

	THIS->set_damping(to<float>(damping));
}
RUCY_END

static
RUCY_DEF0(get_damping)
{
	CHECK;

	return value(THIS->damping());
}
RUCY_END


static Class cReverb;

void
Init_beeps_reverb ()
{
	Module mBeeps = define_module("Beeps");

	cReverb = mBeeps.define_class("Reverb", Beeps::processor_class());
	cReverb.define_alloc_func(alloc);
	cReverb.define_method("mix=",       set_mix);
	cReverb.define_method("mix",        get_mix);
	cReverb.define_method("room_size=", set_room_size);
	cReverb.define_method("room_size",  get_room_size);
	cReverb.define_method("damping=",   set_damping);
	cReverb.define_method("damping",    get_damping);
}


namespace Beeps
{


	Class
	reverb_class ()
	{
		return cReverb;
	}


}// Beeps
