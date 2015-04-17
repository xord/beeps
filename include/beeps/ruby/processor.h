// -*- c++ -*-
#pragma once
#ifndef __BEEPS_RUBY_PROCESSOR_H__
#define __BEEPS_RUBY_PROCESSOR_H__


#include <rucy/rucy.h>
#include <rucy/class.h>
#include <rucy/extension.h>
#include <rucy/exception.h>
#include <beeps/processor.h>


namespace Beeps
{


	Rucy::Class processor_class ();
	// class Beeps::Processor

	Rucy::Class sine_wave_class ();
	// class Beeps::SineWave

	Rucy::Class file_in_class ();
	// class Beeps::FileIn


}// Beeps


RUCY_DECLARE_VALUE_FROM_TO(Beeps::Processor)

RUCY_DECLARE_VALUE_FROM_TO(Beeps::SineWave)

RUCY_DECLARE_VALUE_FROM_TO(Beeps::FileIn)


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
	get_ruby_class<Beeps::FileIn> ()
	{
		return Beeps::file_in_class();
	}


}// Rucy


#endif//EOH