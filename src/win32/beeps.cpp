#include "../beeps.h"


#include <mfapi.h>
#include <xot/windows.h>
#include "exception.h"


namespace Beeps
{


	namespace global
	{

		static bool     initialized = false;

		static bool com_initialized = false;

	}// global


	void
	Beeps_init ()
	{
		if (global::initialized)
			beeps_error(__FILE__, __LINE__, "already initialized.");
		global::initialized = true;

		check_media_foundation_error(
			CoInitializeEx(NULL, COINIT_APARTMENTTHREADED),
			__FILE__, __LINE__);
		global::com_initialized = true;

		check_media_foundation_error(
			MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET),
			__FILE__, __LINE__);
	}

	void
	Beeps_fin ()
	{
		if (!global::initialized)
			beeps_error(__FILE__, __LINE__, "not initialized.");
		global::initialized = false;

		check_media_foundation_error(
			MFShutdown(),
			__FILE__, __LINE__);

		if (global::com_initialized)
		{
			CoUninitialize();
			global::com_initialized = false;
		}
	}


}// Beeps
