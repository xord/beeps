// -*- c++ -*-
#pragma once
#ifndef __BEEPS_RUBY_SOUND_H__
#define __BEEPS_RUBY_SOUND_H__


#include <rucy/class.h>
#include <rucy/extension.h>
#include <beeps/sound.h>


namespace Beeps
{


	Rucy::Class sound_class ();
	// class Beeps::Sound


}// Beeps


RUCY_DECLARE_VALUE_FROM_TO(Beeps::Sound)


namespace Rucy
{


	template <> inline Class
	get_ruby_class<Beeps::Sound> ()
	{
		return Beeps::sound_class();
	}


}// Rucy


#endif//EOH
