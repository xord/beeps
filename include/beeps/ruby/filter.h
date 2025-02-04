// -*- c++ -*-
#pragma once
#ifndef __BEEPS_RUBY_FILTER_H__
#define __BEEPS_RUBY_FILTER_H__


#include <rucy/class.h>
#include <rucy/extension.h>
#include <beeps/filter.h>


RUCY_DECLARE_WRAPPER_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::Gain)

RUCY_DECLARE_WRAPPER_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::Mixer)

RUCY_DECLARE_WRAPPER_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::Envelope)

RUCY_DECLARE_WRAPPER_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::TimeStretch)

RUCY_DECLARE_WRAPPER_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::PitchShift)

RUCY_DECLARE_WRAPPER_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::Analyser)


namespace Beeps
{


	BEEPS_EXPORT Rucy::Class gain_class ();
	// class Beeps::Gain

	BEEPS_EXPORT Rucy::Class mixer_class ();
	// class Beeps::Mixer

	BEEPS_EXPORT Rucy::Class envelope_class ();
	// class Beeps::Envelope

	BEEPS_EXPORT Rucy::Class time_stretch_class ();
	// class Beeps::TimeStretch

	BEEPS_EXPORT Rucy::Class pitch_shift_class ();
	// class Beeps::PitchShift

	BEEPS_EXPORT Rucy::Class analyser_class ();
	// class Beeps::Analyser


}// Beeps


namespace Rucy
{


	template <> inline Class
	get_ruby_class<Beeps::Gain> ()
	{
		return Beeps::gain_class();
	}

	template <> inline Class
	get_ruby_class<Beeps::Mixer> ()
	{
		return Beeps::mixer_class();
	}

	template <> inline Class
	get_ruby_class<Beeps::Envelope> ()
	{
		return Beeps::envelope_class();
	}

	template <> inline Class
	get_ruby_class<Beeps::TimeStretch> ()
	{
		return Beeps::time_stretch_class();
	}

	template <> inline Class
	get_ruby_class<Beeps::PitchShift> ()
	{
		return Beeps::pitch_shift_class();
	}

	template <> inline Class
	get_ruby_class<Beeps::Analyser> ()
	{
		return Beeps::analyser_class();
	}


}// Rucy


#endif//EOH
