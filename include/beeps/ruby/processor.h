// -*- c++ -*-
#pragma once
#ifndef __BEEPS_RUBY_PROCESSOR_H__
#define __BEEPS_RUBY_PROCESSOR_H__


#include <rucy/class.h>
#include <rucy/extension.h>
#include <beeps/processor.h>


RUCY_DECLARE_VALUE_FROM_TO(Beeps::Processor)

RUCY_DECLARE_VALUE_FROM_TO(Beeps::SineWave)

RUCY_DECLARE_VALUE_FROM_TO(Beeps::SquareWave)

RUCY_DECLARE_VALUE_FROM_TO(Beeps::SawtoothWave)

RUCY_DECLARE_VALUE_FROM_TO(Beeps::FileIn)


namespace Beeps
{


	Rucy::Class processor_class ();
	// class Beeps::Processor

	Rucy::Class sine_wave_class ();
	// class Beeps::SineWave

	Rucy::Class square_wave_class ();
	// class Beeps::SineWave

	Rucy::Class sawtooth_wave_class ();
	// class Beeps::SawtoothWave

	Rucy::Class file_in_class ();
	// class Beeps::FileIn


}// Beeps


namespace Rucy
{


	template <> inline Class
	get_ruby_class<Beeps::Processor> ()
	{
		return Beeps::processor_class();
	}

	template <> inline Class
	get_ruby_class<Beeps::SineWave> ()
	{
		return Beeps::sine_wave_class();
	}

	template <> inline Class
	get_ruby_class<Beeps::SquareWave> ()
	{
		return Beeps::square_wave_class();
	}

	template <> inline Class
	get_ruby_class<Beeps::SawtoothWave> ()
	{
		return Beeps::sawtooth_wave_class();
	}

	template <> inline Class
	get_ruby_class<Beeps::FileIn> ()
	{
		return Beeps::file_in_class();
	}


}// Rucy


#endif//EOH
