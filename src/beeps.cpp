// -*- objc -*-
#include "beeps/beeps.h"


#include <time.h>
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
		printf("a %f\n", time() / 60.f);
		OpenAL_init();
		printf("b %f\n", time() / 60.f);

		stk::Stk::setSampleRate(44100);
	}

	void
	fin ()
	{
		printf("c %f\n", time() / 60.f);
		MicIn_clear_streams();
		printf("d %f\n", time() / 60.f);
		SoundPlayer_clear_streams();
		printf("e %f\n", time() / 60.f);

		OpenAL_fin();
		printf("f %f\n", time() / 60.f);
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
