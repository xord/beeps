#include "defs.h"


void Init_beeps ();
void Init_beeps_exception ();

void Init_beeps_sound ();
void Init_beeps_sound_player ();
void Init_beeps_processor ();

void Init_beeps_oscillator ();
void Init_beeps_file_in ();

void Init_beeps_gain ();
void Init_beeps_time_stretch ();
void Init_beeps_pitch_shift ();


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
	Init_beeps_exception();

	Init_beeps_sound();
	Init_beeps_sound_player();
	Init_beeps_processor();

	Init_beeps_oscillator();
	Init_beeps_file_in();

	Init_beeps_gain();
	Init_beeps_time_stretch();
	Init_beeps_pitch_shift();


	RUCY_CATCH
}
