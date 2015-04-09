// -*- objc -*-
#include "beeps/beeps.h"


#include <stdlib.h>
#include <thread>
#include <chrono>
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

	void
	test ()
	{
		ALuint buffer = 0;
		alGenBuffers(1, &buffer);
		check_error(__FILE__, __LINE__);

		int sampling_rate = 44100;
		int seconds       = 1;
		ALsizei bufsize   = sampling_rate * seconds;
		short* samples    = new short[bufsize];
		for (ALsizei i = 0; i < bufsize; ++i)
			samples[i] = rand();
		alBufferData(
			buffer, AL_FORMAT_MONO16, samples, sizeof(short) * bufsize, sampling_rate);
		delete [] samples;

		ALuint source = 0;
		alGenSources(1, &source);
		alSourcei(source, AL_BUFFER, buffer);
		check_error(__FILE__, __LINE__);

		alSourcePlay(source);
		check_error(__FILE__, __LINE__);

		std::this_thread::sleep_for(std::chrono::seconds(seconds));

		alDeleteSources(1, &source);
		alDeleteBuffers(1, &buffer);
	}


}// Beeps
