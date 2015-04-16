// -*- c++ -*-
#pragma once
#ifndef __BEEPS_SRC_OPENAL_H__
#define __BEEPS_SRC_OPENAL_H__


#include <beeps/openal.h>


namespace Beeps
{


	class Sound;


	void play_sound (const Sound& sound);

	ALuint get_sound_buffer_id (const Sound& sound);


}// Beeps


#endif//EOH
