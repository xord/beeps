// -*- c++ -*-
#include "beeps/generator.h"


#include "beeps/exception.h"
#include "../signals.h"


namespace Beeps
{


	struct TextIn::Data
	{

		double sample_rate = 0;

	};// TextIn::Data


	TextIn::TextIn (const char* text, double sample_rate)
	{
		if (sample_rate == 0)
			sample_rate = Beeps::sample_rate();
		if (sample_rate <= 0)
			argument_error(__FILE__, __LINE__);

		self->sample_rate = sample_rate;

		if (text) synthesize(text);
	}

	TextIn::~TextIn ()
	{
	}

	void
	TextIn::synthesize (const char* text)
	{
		not_implemented_error(__FILE__, __LINE__);
	}

	double
	TextIn::sample_rate () const
	{
		return self->sample_rate;
	}

	TextIn::operator bool () const
	{
		if (!Super::operator bool()) return false;
		return self->sample_rate > 0;
	}

	void
	TextIn::generate (Context* context, Signals* signals, uint* offset)
	{
		Super::generate(context, signals, offset);
	}


}// Beeps
