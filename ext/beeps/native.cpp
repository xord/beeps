#include "defs.h"


void Init_beeps ();
void Init_beeps_exception ();

void Init_beeps_signals ();
void Init_beeps_sound ();
void Init_beeps_sound_player ();
void Init_beeps_processor ();

void Init_beeps_oscillator ();
void Init_beeps_sequencer ();
void Init_beeps_file_in ();
void Init_beeps_mic_in ();
void Init_beeps_text_in ();

void Init_beeps_gain ();
void Init_beeps_mixer ();
void Init_beeps_envelope ();
void Init_beeps_low_pass ();
void Init_beeps_high_pass ();
void Init_beeps_reverb ();
void Init_beeps_time_stretch ();
void Init_beeps_pitch_shift ();
void Init_beeps_analyser ();


extern "C" void
Init_beeps_ext ()
{
	RUCY_TRY

	Rucy::init();

	Init_beeps();
	Init_beeps_exception();

	Init_beeps_signals();
	Init_beeps_sound();
	Init_beeps_sound_player();
	Init_beeps_processor();

	Init_beeps_oscillator();
	Init_beeps_sequencer();
	Init_beeps_file_in();
	Init_beeps_mic_in();
	Init_beeps_text_in();

	Init_beeps_gain();
	Init_beeps_mixer();
	Init_beeps_envelope();
	Init_beeps_low_pass();
	Init_beeps_high_pass();
	Init_beeps_reverb();
	Init_beeps_time_stretch();
	Init_beeps_pitch_shift();
	Init_beeps_analyser();

	RUCY_CATCH
}
