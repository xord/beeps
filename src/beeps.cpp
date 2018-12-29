// -*- objc -*-
#include "beeps/beeps.h"


#include "Stk.h"
#include "beeps/exception.h"
#include "openal.h"


namespace Beeps
{


	void
	init ()
	{
		OpenAL_init();

		stk::Stk::setSampleRate(44100);
	}

	void
	fin ()
	{
		OpenAL_fin();
	}

	uint
	sampling_rate ()
	{
		return stk::Stk::sampleRate();
	}


}// Beeps
