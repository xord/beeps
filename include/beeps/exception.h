// -*- c++ -*-
#pragma once
#ifndef __BEEPS_EXCEPTION_H__
#define __BEEPS_EXCEPTION_H__


#include <xot/exception.h>
#include <beeps/defs.h>


namespace Beeps
{


	class BeepsError : public Xot::XotError
	{
		typedef Xot::XotError Super;
		public: BeepsError (const char* str = NULL);
	};


	class OpenALError : public BeepsError
	{
		typedef BeepsError Super;
		public: OpenALError (const char* str = NULL);
	};


	namespace ErrorFunctions
	{

		using namespace Xot::ErrorFunctions;

		[[noreturn]]
		void beeps_error (
			const char* file, int line, const char* format = NULL, ...);

		[[noreturn]]
		void openal_error (
			const char* file, int line, const char* format = NULL, ...);

	}// ErrorFunctions


	using namespace ErrorFunctions;


}// Beeps


#endif//EOH
