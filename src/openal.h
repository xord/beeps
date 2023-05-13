// -*- c++ -*-
#pragma once
#ifndef __BEEPS_SRC_OPENAL_H__
#define __BEEPS_SRC_OPENAL_H__


#if defined(OSX) || defined(IOS)
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#else
	#include <AL/al.h>
	#include <AL/alc.h>
#endif

#include "beeps/defs.h"


namespace Beeps
{


	void OpenAL_init ();

	void OpenAL_fin ();

	ALCenum OpenAL_get_error();

	bool OpenAL_no_error();

	void OpenAL_check_error(const char* file, int line);


}// Beeps


#endif//EOH
