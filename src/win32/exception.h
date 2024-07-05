// -*- c++ -*-
#pragma once
#ifndef __BEEPS_SRC_WIN32_EXCEPTION_H__
#define __BEEPS_SRC_WIN32_EXCEPTION_H__


#include <xot/windows.h>
#include "beeps/exception.h"


namespace Beeps
{


	class MediaFoundationError : public BeepsError
	{
		typedef BeepsError Super;
		public: MediaFoundationError (const char* str = NULL);
	};


	namespace ErrorFunctions
	{

		[[noreturn]]
		void media_foundation_error (
			const char* file, int line,
			HRESULT hresult, const char* format = NULL, ...);

	}// ErrorFunctions


	void check_media_foundation_error (
		HRESULT hresult, const char* file, int line);


}// Beeps


#endif//EOH
