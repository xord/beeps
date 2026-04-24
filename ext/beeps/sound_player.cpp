#include "beeps/ruby/sound.h"


#include "defs.h"


RUCY_DEFINE_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::SoundPlayer)

#define THIS  to<Beeps::SoundPlayer*>(self)

#define CHECK RUCY_CHECK_OBJ(Beeps::SoundPlayer, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return new_type<Beeps::SoundPlayer>(klass);
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
RUCY_DEF0(pause)
{
	CHECK;

	THIS->pause();
	return self;
}
RUCY_END

static
RUCY_DEF0(rewind)
{
	CHECK;

	THIS->rewind();
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
RUCY_DEF0(get_state)
{
	CHECK;

	return value(THIS->state());
}
RUCY_END

static
RUCY_DEF1(set_position, position)
{
	CHECK;

	THIS->set_position(to<uint>(position));
	return position;
}
RUCY_END

static
RUCY_DEF0(get_position)
{
	CHECK;

	return value(THIS->position());
}
RUCY_END

static
RUCY_DEF1(set_time, time)
{
	CHECK;

	THIS->set_time(to<float>(time));
	return time;
}
RUCY_END

static
RUCY_DEF0(get_time)
{
	CHECK;

	return value(THIS->time());
}
RUCY_END

static
RUCY_DEF1(set_time_scale, scale)
{
	CHECK;

	THIS->set_time_scale(to<float>(scale));
	return scale;
}
RUCY_END

static
RUCY_DEF0(get_time_scale)
{
	CHECK;

	return value(THIS->time_scale());
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
RUCY_DEF0(stop_all)
{
	Beeps::stop_all_sound_players();
}
RUCY_END


static Class cSoundPlayer;

void
Init_beeps_sound_player ()
{
	Module mBeeps = define_module("Beeps");

	cSoundPlayer = mBeeps.define_class("SoundPlayer");
	cSoundPlayer.define_alloc_func(alloc);
	cSoundPlayer.define_method("play",   play);
	cSoundPlayer.define_method("pause",  pause);
	cSoundPlayer.define_method("rewind", rewind);
	cSoundPlayer.define_method("stop",   stop);
	cSoundPlayer.define_method("state", get_state);
	cSoundPlayer.define_method("position=",   set_position);
	cSoundPlayer.define_method("position",    get_position);
	cSoundPlayer.define_method("time=",       set_time);
	cSoundPlayer.define_method("time",        get_time);
	cSoundPlayer.define_method("time_scale=", set_time_scale);
	cSoundPlayer.define_method("time_scale",  get_time_scale);
	cSoundPlayer.define_method("gain=", set_gain);
	cSoundPlayer.define_method("gain",  get_gain);
	cSoundPlayer.define_method("loop=", set_loop);
	cSoundPlayer.define_method("loop",  get_loop);
	cSoundPlayer.define_singleton_method("stop_all", stop_all);

	cSoundPlayer.define_const("STATE_UNKNOWN", Beeps::SoundPlayer::STATE_UNKNOWN);
	cSoundPlayer.define_const("PLAYING",       Beeps::SoundPlayer::PLAYING);
	cSoundPlayer.define_const("PAUSED",        Beeps::SoundPlayer::PAUSED);
	cSoundPlayer.define_const("STOPPED",       Beeps::SoundPlayer::STOPPED);
}


namespace Beeps
{


	Class
	sound_player_class ()
	{
		return cSoundPlayer;
	}


}// Beeps
