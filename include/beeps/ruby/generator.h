// -*- c++ -*-
#pragma once
#ifndef __BEEPS_RUBY_GENERATOR_H__
#define __BEEPS_RUBY_GENERATOR_H__


#include <rucy/class.h>
#include <rucy/extension.h>
#include <beeps/generator.h>


RUCY_DECLARE_WRAPPER_VALUE_FROM_TO(Beeps::Oscillator)

RUCY_DECLARE_WRAPPER_VALUE_FROM_TO(Beeps::FileIn)


namespace Beeps
{


	Rucy::Class oscillator_class ();
	// class Beeps::Oscillator

	Rucy::Class file_in_class ();
	// class Beeps::FileIn


}// Beeps


namespace Rucy
{


	template <> inline Class
	get_ruby_class<Beeps::Oscillator> ()
	{
		return Beeps::oscillator_class();
	}

	template <> inline Class
	get_ruby_class<Beeps::FileIn> ()
	{
		return Beeps::file_in_class();
	}


}// Rucy


#endif//EOH
