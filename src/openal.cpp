#include "openal.h"


#include <assert.h>
#include "beeps/exception.h"
#include "beeps/debug.h"
#include "sound.h"


namespace Beeps
{


	namespace global
	{

		static ALCdevice* device   = NULL;

		static ALCcontext* context = NULL;

	}// global


	static void
	cleanup ()
	{
		alcMakeContextCurrent(NULL);

		if (global::context)
		{
			alcDestroyContext(global::context);
			global::context = NULL;
		}

		if (global::device)
		{
			alcCloseDevice(global::device);
			global::device = NULL;
		}
	}

	void
	OpenAL_init ()
	{
		if (global::device || global::context)
			beeps_error(__FILE__, __LINE__, "already initialized.");

		global::device = alcOpenDevice(NULL);
		if (!global::device) goto FAILED;

		global::context = alcCreateContext(global::device, NULL);
		if (!global::context) goto FAILED;

		if (!alcMakeContextCurrent(global::context))
			goto FAILED;

		return;

	FAILED:
		cleanup();
		openal_error(__FILE__, __LINE__, "failed to initialize OpenAL.");
	}

	void
	OpenAL_fin ()
	{
		if (!global::context)
			beeps_error(__FILE__, __LINE__, "not initialized.");

		cleanup();
	}

	void*
	OpenAL_get_context ()
	{
		return global::context;
	}

	ALCenum
	OpenAL_get_error ()
	{
		assert(global::device);

		return alcGetError(global::device);
	}

	bool
	OpenAL_no_error ()
	{
		return OpenAL_get_error() == ALC_NO_ERROR;
	}

	static String
	get_error_name (ALenum error)
	{
		switch (error)
		{
			case ALC_NO_ERROR:        return "ALC_NO_ERROR";
			case ALC_INVALID_DEVICE:  return "ALC_INVALID_DEVICE";
			case ALC_INVALID_CONTEXT: return "ALC_INVALID_CONTEXT";
			case ALC_INVALID_ENUM:    return "ALC_INVALID_ENUM";
			case ALC_INVALID_VALUE:   return "ALC_INVALID_VALUE";
			case ALC_OUT_OF_MEMORY:   return "ALC_OUT_OF_MEMORY";
			default:                  return "UNKNOWN ERROR";
		}
	}

	void
	OpenAL_check_error (const char* file, int line)
	{
		ALCenum e = OpenAL_get_error();
		if (e != ALC_NO_ERROR)
			openal_error(file, line, "OpenAL error %s", get_error_name(e).c_str());
	}


}// Beeps
