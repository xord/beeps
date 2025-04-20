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

		Signals input_signals;

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

		auto& in  = self->input_signals;
		auto& out = *signals;

		Signals_fill(&out, out.capacity(), 0);

		if (!in)
			in = Signals_create(out.capacity(), out.nchannels());
		else
			Signals_clear(&in, out.capacity());

		uint out_size = 0;
		for (auto& input : self->inputs)
		{
			Signals_clear(&in);

			uint input_offset = *offset;
			Processor_get_context(context)->process(input, &in, &input_offset);

			uint size = input_offset - *offset;
			if (size > out_size) out_size = size;

			Signals_add(&out, in);
		}

		Signals_set_nsamples(&out, out_size);
		*offset += out_size;
	}

	Mixer::operator bool () const
	{
		if (self->inputs.empty()) return false;

		for (auto& input : self->inputs)
			if (!*input) return false;
		return true;
	}


}// Beeps
