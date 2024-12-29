// -*- c++ -*-
#pragma once
#ifndef __BEEPS_RUBY_GENERATOR_H__
#define __BEEPS_RUBY_GENERATOR_H__


#include <rucy/class.h>
#include <rucy/extension.h>
#include <beeps/generator.h>


RUCY_DECLARE_WRAPPER_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::Oscillator)

RUCY_DECLARE_WRAPPER_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::Sequencer)

RUCY_DECLARE_WRAPPER_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::FileIn)

RUCY_DECLARE_WRAPPER_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::MicIn)


namespace Beeps
{


	BEEPS_EXPORT Rucy::Class oscillator_class ();
	// class Beeps::Oscillator

	BEEPS_EXPORT Rucy::Class sequencer_class ();
	// class Beeps::Sequencer

	BEEPS_EXPORT Rucy::Class file_in_class ();
	// class Beeps::FileIn

	BEEPS_EXPORT Rucy::Class mic_in_class ();
	// class Beeps::MicIn


}// Beeps


namespace Rucy
{


	template <> inline Class
	get_ruby_class<Beeps::Oscillator> ()
	{
		return Beeps::oscillator_class();
	}

	template <> inline Class
	get_ruby_class<Beeps::Sequencer> ()
	{
		return Beeps::sequencer_class();
	}

	template <> inline Class
	get_ruby_class<Beeps::FileIn> ()
	{
		return Beeps::file_in_class();
	}

	template <> inline Class
	get_ruby_class<Beeps::MicIn> ()
	{
		return Beeps::mic_in_class();
	}


}// Rucy


#endif//EOH
