#include "beeps/filter.h"


#include <algorithm>
#include <vector>
#include "processor.h"
#include "signals.h"


namespace Beeps
{


	struct Mixer::Data
	{

		std::vector<Processor::Ref> inputs;

	};// Mixer::Data


	Mixer::Mixer (Processor* input)
	{
		if (input) add_input(input);
	}

	Mixer::~Mixer ()
	{
	}

	void
	Mixer::add_input (Processor* input)
	{
		auto it = std::find(self->inputs.begin(), self->inputs.end(), input);
		if (it != self->inputs.end()) return;

		self->inputs.emplace_back(input);

		set_updated();
	}

	void
	Mixer::remove_input (Processor* input)
	{
		auto it = std::find(self->inputs.begin(), self->inputs.end(), input);
		if (it == self->inputs.end()) return;

		self->inputs.erase(it);

		set_updated();
	}

	Mixer::iterator
	Mixer::begin ()
	{
		return self->inputs.begin();
	}

	Mixer::const_iterator
	Mixer::begin () const
	{
		return self->inputs.begin();
	}

	Mixer::iterator
	Mixer::end ()
	{
		return self->inputs.end();
	}

	Mixer::const_iterator
	Mixer::end () const
	{
		return self->inputs.end();
	}

	void
	Mixer::filter (Context* context, Signals* signals, uint* offset)
	{
		Super::filter(context, signals, offset);

		Signals_resize(signals, signals->capacity(), 0);

		Signals sig   = Signals_create(signals->capacity(), signals->nchannels());
		uint min_size = signals->capacity();
		for (auto& input : self->inputs)
		{
			Signals_clear(&sig);

			uint sig_offset = *offset;
			Processor_get_context(context)->process(input, &sig, &sig_offset);

			uint size = sig_offset - *offset;
			if (size < min_size) min_size = size;

			Signals_add(signals, sig);
		}

		*offset += min_size;
	}

	Mixer::operator bool () const
	{
		if (self->inputs.empty()) return false;

		for (auto& input : self->inputs)
			if (!*input) return false;
		return true;
	}


}// Beeps
