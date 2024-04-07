#include "beeps/ruby/filter.h"


#include "beeps/ruby/processor.h"
#include "defs.h"


RUCY_DEFINE_WRAPPER_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::ADSR)

#define THIS  to<Beeps::ADSR*>(self)

#define CHECK RUCY_CHECK_OBJ(Beeps::ADSR, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return value(new Beeps::RubyProcessor<Beeps::ADSR>, klass);
}
RUCY_END

static
RUCY_DEF1(note_on, delay)
{
	CHECK;

	THIS->note_on(to<float>(delay));
}
RUCY_END

static
RUCY_DEF1(note_off, delay)
{
	CHECK;

	THIS->note_off(to<float>(delay));
}
RUCY_END

static
RUCY_DEF1(set_attack_time, time)
{
	CHECK;

	THIS->set_attack_time(to<float>(time));
	return time;
}
RUCY_END

static
RUCY_DEF0(get_attack_time)
{
	CHECK;

	return value(THIS->attack_time());
}
RUCY_END

static
RUCY_DEF1(set_decay_time, time)
{
	CHECK;

	THIS->set_decay_time(to<float>(time));
	return time;
}
RUCY_END

static
RUCY_DEF0(get_decay_time)
{
	CHECK;

	return value(THIS->decay_time());
}
RUCY_END

static
RUCY_DEF1(set_sustain_level, level)
{
	CHECK;

	THIS->set_sustain_level(to<float>(level));
	return level;
}
RUCY_END

static
RUCY_DEF0(get_sustain_level)
{
	CHECK;

	return value(THIS->sustain_level());
}
RUCY_END

static
RUCY_DEF1(set_release_time, time)
{
	CHECK;

	THIS->set_release_time(to<float>(time));
	return time;
}
RUCY_END

static
RUCY_DEF0(get_release_time)
{
	CHECK;

	return value(THIS->release_time());
}
RUCY_END


static Class cADSR;

void
Init_beeps_adsr ()
{
	Module mBeeps = define_module("Beeps");

	cADSR = mBeeps.define_class("ADSR", Beeps::processor_class());
	cADSR.define_alloc_func(alloc);
	cADSR.define_private_method("note_on!",  note_on);
	cADSR.define_private_method("note_off!", note_off);
	cADSR.define_method( "attack_time=",   set_attack_time);
	cADSR.define_method( "attack_time",    get_attack_time);
	cADSR.define_method(  "decay_time=",    set_decay_time);
	cADSR.define_method(  "decay_time",     get_decay_time);
	cADSR.define_method("sustain_level=", set_sustain_level);
	cADSR.define_method("sustain_level",  get_sustain_level);
	cADSR.define_method("release_time=",  set_release_time);
	cADSR.define_method("release_time",   get_release_time);
}


namespace Beeps
{


	Class
	adsr_class ()
	{
		return cADSR;
	}


}// Beeps
