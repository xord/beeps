// -*- objc -*-
#include "beeps/beeps.h"


#include <stdlib.h>
#include "Stk.h"
#include "beeps/openal.h"
#include "beeps/exception.h"


namespace Beeps
{


	namespace global
	{

		static ALCdevice* device   = NULL;

		static ALCcontext* context = NULL;

	}// global


	namespace g = global;


	static void
	cleanup ()
	{
		void cleanup_sources ();
		cleanup_sources();

		alcMakeContextCurrent(NULL);

		if (g::context)
		{
			alcDestroyContext(g::context);
			g::context = NULL;
		}

		if (g::device)
		{
			alcCloseDevice(g::device);
			g::device = NULL;
		}
	}

	void
	init ()
	{
		if (g::device || g::context)
			beeps_error(__FILE__, __LINE__, "Beeps::init(): already initialized.");

		stk::Stk::setSampleRate(44100);

		g::device = alcOpenDevice(NULL);
		if (!g::device) goto FAILED;

		g::context = alcCreateContext(g::device, NULL);
		if (!g::context) goto FAILED;

		if (!alcMakeContextCurrent(g::context))
			goto FAILED;

		return;

	FAILED:
		cleanup();
		openal_error(__FILE__, __LINE__, "failed to setup OpenAL.");
	}

	void
	fin ()
	{
		if (!g::context)
			beeps_error(__FILE__, __LINE__, "Beeps::fin(): not initialized.");

		cleanup();
	}

	ALCdevice*
	get_device ()
	{
		return g::device;
	}


	uint
	sampling_rate ()
	{
		return stk::Stk::sampleRate();
	}


}// Beeps
