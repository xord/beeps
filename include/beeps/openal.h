// -*- c++ -*-
#pragma once
#ifndef __BEEPS_OPENAL_H__
#define __BEEPS_OPENAL_H__


#if defined(OSX) || defined(IOS)
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#else
	#include <AL/al.h>
	#include <AL/alc.h>
#endif


namespace Beeps
{


	ALCenum get_error ();

	bool no_error ();

	bool is_error (ALCenum err);

	void check_error(const char* file, int line);

	void clear_error ();


}// Beeps


#endif//EOH
