// -*- objc -*-
#include "beeps/beeps.h"


#include "Stk.h"
#include "beeps/exception.h"
#include "openal.h"
#include "sound.h"


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

	void
	process_streams ()
	{
		SoundPlayer_process_streams();
	}

	double
	sample_rate ()
	{
		return stk::Stk::sampleRate();
	}


}// Beeps
