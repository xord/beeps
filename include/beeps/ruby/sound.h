// -*- c++ -*-
#pragma once
#ifndef __BEEPS_RUBY_SOUND_H__
#define __BEEPS_RUBY_SOUND_H__


#include <rucy/class.h>
#include <rucy/extension.h>
#include <beeps/sound.h>


RUCY_DECLARE_VALUE_FROM_TO(Beeps::SoundPlayer)

RUCY_DECLARE_VALUE_FROM_TO(Beeps::Sound)


namespace Beeps
{


	Rucy::Class sound_player_class ();
	// class Beeps::SoundPlayer

	Rucy::Class sound_class ();
	// class Beeps::Sound


}// Beeps


namespace Rucy
{


	template <> inline Class
	get_ruby_class<Beeps::SoundPlayer> ()
	{
		return Beeps::sound_player_class();
	}

	template <> inline Class
	get_ruby_class<Beeps::Sound> ()
	{
		return Beeps::sound_class();
	}


}// Rucy


#endif//EOH
