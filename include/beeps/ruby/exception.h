// -*- c++ -*-
#pragma once
#ifndef __BEEPS_RUBY_EXCEPTION_H__
#define __BEEPS_RUBY_EXCEPTION_H__


#include <rucy/class.h>
#include <beeps/exception.h>


namespace Beeps
{


	BEEPS_EXPORT Rucy::Class beeps_error_class ();
	// class Beeps::BeepsError

	BEEPS_EXPORT Rucy::Class openal_error_class ();
	// class Beeps::OpenALError


}// Beeps


#endif//EOH
