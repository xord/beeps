#include "beeps/processor.h"


#include "beeps/exception.h"
#include "signals.h"


namespace Beeps
{


	Processor::Processor ()
	{
	}

	Processor::~Processor ()
	{
	}

	void
	Processor::process (Signals* signals)
	{
		if (!signals || !*signals)
			argument_error(__FILE__, __LINE__);

		if (!*this)
			invalid_state_error(__FILE__, __LINE__);
	}

	Processor::operator bool () const
	{
		return true;
	}

	bool
	Processor::operator ! () const
	{
		return !operator bool();
	}


}// Beeps
