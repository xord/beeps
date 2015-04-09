#include <rucy.h>
#include "defs.h"


using namespace Rucy;


void Init_beeps ();


extern "C" void
#ifdef COCOAPODS
	Init_beeps_native ()
#else
	Init_native ()
#endif
{
	RUCY_TRY

	Rucy::init();

	Init_beeps();

	RUCY_CATCH
}
