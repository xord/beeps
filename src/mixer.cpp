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

	Mixer::operator bool () const
	{
		const auto* input = this->input();
		if (input && !*input)
			return false;

		if (!input && self->inputs.empty())
			return false;

		for (auto& i : self->inputs)
			if (!*i) return false;

		return true;
	}

	static void
	mix (
		Mixer* mixer, Processor* input,
		Processor::Context* context, Signals* signals, uint offset)
	{
		Mixer::Data* self = mixer->self.get();

		auto& insig = self->input_signals;
		if (!insig)
			insig = Signals_create(signals->capacity(), signals->nchannels());
		else
			Signals_clear(&insig, signals->capacity());

		Processor_get_context(context)->process(input, &insig, &offset);

		if (insig.nsamples() > signals->nsamples())
			Signals_set_nsamples(signals, insig.nsamples());

		Signals_add(signals, insig);
	}

	void
	Mixer::filter (Context* context, Signals* signals, uint* offset)
	{
		Signals_fill(signals, signals->capacity(), 0);
		Signals_clear(signals);

		Super::filter(context, signals, offset);

		for (auto& input : self->inputs)
			mix(this, input.get(), context, signals, *offset);

		*offset += signals->nsamples();
	}


}// Beeps
