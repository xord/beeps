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
		printf("a %lf\n", (double) time(NULL));
		OpenAL_init();
		printf("b %lf\n", (double) time(NULL));

		stk::Stk::setSampleRate(44100);
	}

	void
	fin ()
	{
		printf("c %lf\n", (double) time(NULL));
		MicIn_clear_streams();
		printf("d %lf\n", (double) time(NULL));
		SoundPlayer_clear_streams();
		printf("e %lf\n", (double) time(NULL));

		OpenAL_fin();
		printf("f %lf\n", (double) time(NULL));
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
