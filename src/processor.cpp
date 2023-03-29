#include "beeps/processor.h"


#include "beeps/exception.h"
#include "signals.h"


namespace Beeps
{


	struct Processor::Data
	{

		bool need_input = false;

		Processor::Ref input;

	};// Processor::Data


	Processor::Processor (bool input)
	{
		self->need_input = input;
	}

	Processor::~Processor ()
	{
	}

	void
	Processor::set_input (Processor* input)
	{
		if (!self->need_input)
			invalid_state_error(__FILE__, __LINE__, "can not have inputs");

		self->input = input;
	}

	const Processor*
	Processor::input () const
	{
		return self->input;
	}

	void
	Processor::process (Signals* signals)
	{
		if (!signals || !*signals)
			argument_error(__FILE__, __LINE__);

		if (!*this)
			invalid_state_error(__FILE__, __LINE__);

		if (self->input)
			self->input->process(signals);
	}

	Processor::operator bool () const
	{
		return !self->need_input || (self->input && *self->input);
	}

	bool
	Processor::operator ! () const
	{
		return !operator bool();
	}


}// Beeps
