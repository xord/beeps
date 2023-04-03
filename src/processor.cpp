#include "beeps/processor.h"


#include "beeps/exception.h"
#include "signals.h"


namespace Beeps
{


	struct Processor::Data
	{

		bool generator = false;

		Processor::Ref input;

		bool has_generator () const
		{
			return generator || (input && input->self->has_generator());
		}

	};// Processor::Data


	Processor::Processor (bool generator)
	{
		self->generator = generator;
	}

	Processor::~Processor ()
	{
	}

	void
	Processor::reset ()
	{
		if (input) input->reset();
	}

	void
	Processor::set_input (Processor* input)
	{
		if (input && self->generator)
			invalid_state_error(__FILE__, __LINE__, "generator cannot have inputs");

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
		return self->has_generator();
	}

	bool
	Processor::operator ! () const
	{
		return !operator bool();
	}


}// Beeps
