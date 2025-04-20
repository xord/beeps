// -*- objc -*-
#include "beeps.h"


#include "Stk.h"
#include "beeps/exception.h"
#include "beeps/debug.h"
#include "openal.h"
#include "sound.h"
#include "mic_in.h"


namespace Beeps
{


	void
	init ()
	{
		Beeps_init();
		OpenAL_init();

		stk::Stk::setSampleRate(44100);
	}

	void
	fin ()
	{
		MicIn_clear_streams();
		SoundPlayer_clear_streams();

		OpenAL_fin();
		Beeps_fin();
	}

	double
	sample_rate ()
	{
		return stk::Stk::sampleRate();
	}

	void
	process_streams ()
	{
		MicIn_process_streams();
		SoundPlayer_process_streams();
	}


}// Beeps
