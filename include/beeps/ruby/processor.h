// -*- c++ -*-
#pragma once
#ifndef __BEEPS_RUBY_PROCESSOR_H__
#define __BEEPS_RUBY_PROCESSOR_H__


#include <rucy/class.h>
#include <rucy/extension.h>
#include <beeps/processor.h>


RUCY_DECLARE_VALUE_FROM_TO(Beeps::Processor)

RUCY_DECLARE_VALUE_FROM_TO(Beeps::Oscillator)

RUCY_DECLARE_VALUE_FROM_TO(Beeps::TimeStretch)

RUCY_DECLARE_VALUE_FROM_TO(Beeps::FileIn)


namespace Beeps
{


	Rucy::Class processor_class ();
	// class Beeps::Processor

	Rucy::Class oscillator_class ();
	// class Beeps::Oscillator

	Rucy::Class time_stretch_class ();
	// class Beeps::TimeStretch

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
	get_ruby_class<Beeps::Oscillator> ()
	{
		return Beeps::oscillator_class();
	}

	template <> inline Class
	get_ruby_class<Beeps::TimeStretch> ()
	{
		return Beeps::time_stretch_class();
	}

	template <> inline Class
	get_ruby_class<Beeps::FileIn> ()
	{
		return Beeps::file_in_class();
	}


}// Rucy


#endif//EOH
