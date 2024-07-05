#include "../beeps.h"


#include <mfapi.h>
#include <xot/windows.h>
#include "exception.h"


namespace Beeps
{


	void
	Beeps_init ()
	{
		check_media_foundation_error(
			CoInitializeEx(NULL, COINIT_MULTITHREADED),
			__FILE__, __LINE__);

		check_media_foundation_error(
			MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET),
			__FILE__, __LINE__);
	}

	void
	Beeps_fin ()
	{
		check_media_foundation_error(
			MFShutdown(),
			__FILE__, __LINE__);

		CoUninitialize();
	}


}// Beeps
