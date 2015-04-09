#include "beeps/openal.h"


#include "beeps/defs.h"
#include "beeps/exception.h"


namespace Beeps
{


	ALCdevice* get_device ();


	ALCenum
	get_error ()
	{
		return alcGetError(get_device());
	}

	bool
	no_error ()
	{
		return get_error() == ALC_NO_ERROR;
	}

	bool
	is_error (ALCenum err)
	{
		return get_error() == err;
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
			case ALC_OUT_OF_MEMORY:   return "ALC_OUT_OF_MEMORY";
		}
		return "UNKNOWN ERROR";
	}

	void
	check_error (const char* file, int line)
	{
		ALCenum e = get_error();
		if (e != ALC_NO_ERROR)
			openal_error(file, line, "OpenAL error %s", get_error_name(e).c_str());
	}

	void
	clear_error ()
	{
		get_error();
	}


}// Beeps
