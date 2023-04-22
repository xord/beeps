// -*- objc -*-
#include "beeps/beeps.h"


#include "Stk.h"
#include "beeps/exception.h"
#include "openal.h"
#include "sound.h"
#include "mic_in.h"


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
		MicIn_clear_streams();
		SoundPlayer_clear_streams();

		OpenAL_fin();
	}

	void
	process_streams ()
	{
		MicIn_process_streams();
		SoundPlayer_process_streams();
	}

	double
	sample_rate ()
	{
		return stk::Stk::sampleRate();
	}


}// Beeps
