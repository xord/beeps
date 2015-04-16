#include <rucy.h>
#include "defs.h"


using namespace Rucy;


void Init_beeps ();
void Init_sound ();
void Init_processor ();
void Init_sine_wave ();
void Init_file_input ();


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
	Init_sound();
	Init_processor();
	Init_sine_wave();
	Init_file_input();

	RUCY_CATCH
}
