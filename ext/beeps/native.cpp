#include "defs.h"


void Init_beeps ();

void Init_processor ();
void Init_sine_wave ();
void Init_square_wave ();
void Init_sawtooth_wave ();
void Init_file_in ();

void Init_sound ();


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

	Init_processor();
	Init_sine_wave();
	Init_square_wave();
	Init_sawtooth_wave();
	Init_file_in();

	Init_sound();

	RUCY_CATCH
}
