// -*- c++ -*-
#pragma once
#ifndef __BEEPS_RUBY_SIGNALS_H__
#define __BEEPS_RUBY_SIGNALS_H__


#include <rucy/class.h>
#include <rucy/extension.h>
#include <beeps/signals.h>


RUCY_DECLARE_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::Signals)


namespace Beeps
{


	BEEPS_EXPORT Rucy::Class signals_class ();
	// class Beeps::Signals


}// Beeps


namespace Rucy
{


	template <> inline Class
	get_ruby_class<Beeps::Signals> ()
	{
		return Beeps::signals_class();
	}


}// Rucy


#endif//EOH
