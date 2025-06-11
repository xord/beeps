#include "beeps/generator.h"


#include "beeps/exception.h"
#include "signals.h"


namespace Beeps
{


	struct TextIn::Data
	{

		Signals signals;

	};// TextIn::Data


	TextIn::TextIn (const char* text, double sample_rate)
	{
		self->signals = Signals_create(1024 * 10, 1, sample_rate);
	}

	TextIn::~TextIn ()
	{
	}

	double
	TextIn::sample_rate () const
	{
		return self->signals.sample_rate();
	}

	TextIn::operator bool () const
	{
		if (!Super::operator bool()) return false;
		return self->signals;
	}

	void
	TextIn::generate (Context* context, Signals* signals, uint* offset)
	{
		Super::generate(context, signals, offset);

		*offset += Signals_copy(signals, self->signals, *offset);
	}


}// Beeps
