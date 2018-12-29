#include "beeps/exception.h"


namespace Beeps
{


	BeepsError::BeepsError (const char* str)
	:	Super(str)
	{
	}


	OpenALError::OpenALError (const char* str)
	:	Super(str)
	{
	}


	namespace ErrorFunctions
	{

		void
		beeps_error (const char* file, int line, const char* format, ...)
		{
			XOT_STRINGF(format, s);
			throw BeepsError(Xot::error_text(file, line, s));
		}

		void
		openal_error (const char* file, int line, const char* format, ...)
		{
			XOT_STRINGF(format, s);
			throw OpenALError(Xot::error_text(file, line, s));
		}

	}// ErrorFunctions


}// Beeps
