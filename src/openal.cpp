#include "openal.h"


#include <assert.h>
#include <algorithm>
#include "beeps/exception.h"
#include "beeps/debug.h"
#include "sound.h"


namespace Beeps
{


	namespace global
	{

		static ALCdevice* device        = NULL;

		static ALCcontext* context      = NULL;

		static uint max_playing_sources = 0;

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

	static uint
	get_max_playing_sources ()
	{
#if defined(OSX) || defined(IOS)
		typedef ALint (*GetMaxBusses) ();

		auto get_max_busses =
			(GetMaxBusses) alcGetProcAddress(NULL, "alcMacOSXGetMixerMaxiumumBusses");
		if (get_max_busses)
			return (uint) std::max(get_max_busses(), 0);
#endif

		return 0;
	}

	void
	OpenAL_init ()
	{
		if (global::device)
			beeps_error(__FILE__, __LINE__, "already initialized.");
		if (global::context)
			beeps_error(__FILE__, __LINE__, "already initialized.");

		global::device = alcOpenDevice(NULL);
		if (!global::device) goto FAILED;

		global::context = alcCreateContext(global::device, NULL);
		if (!global::context) goto FAILED;

		if (!alcMakeContextCurrent(global::context))
			goto FAILED;

		global::max_playing_sources = get_max_playing_sources();
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

	uint
	OpenAL_get_max_playing_sources ()
	{
		return global::max_playing_sources;
	}

	ALenum
	OpenAL_get_error ()
	{
		assert(global::context);

		return alGetError();
	}

	bool
	OpenAL_no_error ()
	{
		return OpenAL_get_error() == AL_NO_ERROR;
	}

	static String
	get_error_name (ALenum error)
	{
		switch (error)
		{
			case AL_NO_ERROR:          return "AL_NO_ERROR";
			case AL_INVALID_NAME:      return "AL_INVALID_NAME";
			case AL_INVALID_ENUM:      return "AL_INVALID_ENUM";
			case AL_INVALID_VALUE:     return "AL_INVALID_VALUE";
			case AL_INVALID_OPERATION: return "AL_INVALID_OPERATION";
			case AL_OUT_OF_MEMORY:     return "AL_OUT_OF_MEMORY";
			default:                   return "UNKNOWN ERROR";
		}
	}

	static String
	get_device_error_name (ALCenum error)
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
		ALenum e = OpenAL_get_error();
		if (e != AL_NO_ERROR)
		{
			openal_error(
				file, line, "OpenAL error %s (0x%x)",
				get_error_name(e).c_str(), (uint) e);
		}
	}

	void
	OpenAL_check_error (ALCdevice* device, const char* file, int line)
	{
		assert(device);

		ALCenum e = alcGetError(device);
		if (e != ALC_NO_ERROR)
		{
			openal_error(
				file, line, "OpenAL error %s (0x%x)",
				get_device_error_name(e).c_str(), (uint) e);
		}
	}


}// Beeps
