#include "exception.h"


#include <xot/string.h>


namespace Beeps
{


	MediaFoundationError::MediaFoundationError (const char* str)
	:	Super(str)
	{
	}


	namespace ErrorFunctions
	{

		void
		media_foundation_error (
			const char* file, int line, HRESULT hresult, const char* format, ...)
		{
			XOT_STRINGF(format, s);
			throw MediaFoundationError(
				Xot::error_text(file, line, s + Xot::stringf(" (0x%p)", hresult)));
		}

	}// ErrorFunctions


	void
	check_media_foundation_error (HRESULT hresult, const char* file, int line)
	{
		if (FAILED(hresult))
			media_foundation_error(file, line, hresult);
	}


}// Beeps
