#include "processor.h"


#include <assert.h>
#include <xot/time.h>
#include "beeps/exception.h"
#include "beeps/debug.h"


namespace Beeps
{


	struct Processor::Data
	{

		bool generator = false;

		float buffering_seconds = 0;

		double last_update_time = 0;

		Processor::Ref input;

		bool has_generator () const
		{
			return generator || (input && input->self->has_generator());
		}

	};// Processor::Data


	ProcessorContext*
	Processor_get_context(Processor::Context* context)
	{
		return (ProcessorContext*) context;
	}

	float
	Processor_get_buffering_seconds (Processor* processor)
	{
		assert(processor);

		return processor->self->buffering_seconds;
	}


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
		if (self->input) self->input->reset();

		set_updated();
	}

	void
	Processor::set_input (Processor* input)
	{
		if (input && self->generator)
			invalid_state_error(__FILE__, __LINE__, "generator cannot have inputs");

		self->input = input;

		set_updated();
	}

	Processor*
	Processor::input ()
	{
		return self->input;
	}

	const Processor*
	Processor::input () const
	{
		return const_cast<Processor*>(this)->input();
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

	void
	Processor::process (Context* context, Signals* signals, uint* offset)
	{
		if (self->generator)
			generate(context, signals, offset);
		else
			filter(context, signals, offset);
	}

	void
	Processor::generate (Context* context, Signals* signals, uint* offset)
	{
		if (!context)
			argument_error(__FILE__, __LINE__);
		if (!signals)
			argument_error(__FILE__, __LINE__);
		if (!*signals)
			argument_error(__FILE__, __LINE__);
		if (signals->nsamples() > 0)
			argument_error(__FILE__, __LINE__);
		if (!offset)
			argument_error(__FILE__, __LINE__);

		if (!*this)
			invalid_state_error(__FILE__, __LINE__);
		if (self->input)
			invalid_state_error(__FILE__, __LINE__);
	}

	void
	Processor::filter (Context* context, Signals* signals, uint* offset)
	{
		if (!context)
			argument_error(__FILE__, __LINE__);
		if (!signals)
			argument_error(__FILE__, __LINE__);
		if (!*signals)
			argument_error(__FILE__, __LINE__);
		if (!offset)
			argument_error(__FILE__, __LINE__);

		if (!*this)
			invalid_state_error(__FILE__, __LINE__);

		if (self->input)
			Processor_get_context(context)->process(self->input, signals, offset);
	}

	void
	Processor::set_updated ()
	{
		self->last_update_time = Xot::time();
	}


	Generator::Generator ()
	:	Super(true)
	{
	}

	void
	Generator::filter (Context* context, Signals* signals, uint* offset)
	{
		beeps_error(__FILE__, __LINE__);
	}


	Filter::Filter (Processor* input)
	:	Super(false)
	{
		if (input) set_input(input);
	}

	void
	Filter::set_buffering_seconds (float seconds)
	{
		Super::self->buffering_seconds = seconds;

		set_updated();
	}

	void
	Filter::generate (Context* context, Signals* signals, uint* offset)
	{
		beeps_error(__FILE__, __LINE__);
	}


	SignalsBuffer::SignalsBuffer (
		uint nsamples_per_block, uint nchannels, double sample_rate)
	{
		buffer = Signals_create(nsamples_per_block, nchannels, sample_rate);
		assert(*this);
	}

	void
	SignalsBuffer::process (
		ProcessorContext* context,
		Processor* processor, Signals* signals, uint* offset)
	{
		assert(processor && context && signals && offset);

		if (
			last_update_time < processor->self->last_update_time ||
			*offset < buffer_offset)
		{
			clear();
		}

		if (buffer.nsamples() == 0)
			buffer_next(context, processor, *offset);

		while (true)
		{
			*offset += Signals_copy(signals, buffer, *offset - buffer_offset);

			bool signals_full = signals->nsamples() == signals->capacity();
			bool  buffer_full =   buffer.nsamples() ==   buffer.capacity();
			if (signals_full || !buffer_full)
				break;

			buffer_next(context, processor, buffer_offset + buffer.nsamples());
		}
	}

	SignalsBuffer::operator bool () const
	{
		return buffer;
	}

	bool
	SignalsBuffer::operator ! () const
	{
		return !operator bool();
	}

	void
	SignalsBuffer::buffer_next (
		ProcessorContext* context, Processor* processor, uint offset)
	{
		Signals_clear(&buffer);
		buffer_offset = offset;
		context->process(processor, &buffer, &offset, true);

		last_update_time = Xot::time();
	}

	void
	SignalsBuffer::clear ()
	{
		Signals_clear(&buffer);
		buffer_offset = 0;
	}


	ProcessorContext::ProcessorContext (uint nchannels, double sample_rate)
	:	sample_rate(sample_rate), nchannels(nchannels)
	{
	}

	void
	ProcessorContext::process (
		Processor* processor, Signals* signals, uint* offset, bool ignore_buffer)
	{
		assert(processor);

		SignalsBuffer* buffer = NULL;
		if (!ignore_buffer && (buffer = get_buffer(processor)))
			buffer->process(this, processor, signals, offset);
		else
			processor->process(this, signals, offset);
	}

	static uintptr_t
	get_buffer_key (Processor* processor)
	{
		assert(processor);

		return (uintptr_t) processor->self.get();
	}

	SignalsBuffer*
	ProcessorContext::get_buffer (Processor* processor)
	{
		float buffering_sec = Processor_get_buffering_seconds(processor);
		if (buffering_sec <= 0) return NULL;

		uintptr_t key = get_buffer_key(processor);
		auto it       = buffers.find(key);
		if (it != buffers.end()) return it->second.get();

		SignalsBuffer* buffer =
			new SignalsBuffer(buffering_sec * sample_rate, nchannels, sample_rate);

		buffers.emplace(key, buffer);
		return buffer;
	}


	StreamContext::StreamContext (
		uint nsamples_per_process, uint nchannels, double sample_rate)
	:	context(nchannels, sample_rate),
		signals(Signals_create(nsamples_per_process, nchannels, sample_rate))
	{
	}

	Signals
	StreamContext::process_next (Processor* processor)
	{
		assert(processor);

		Signals_clear(&signals);
		context.process(processor, &signals, &offset);

		if (signals.nsamples() < signals.capacity())
			finished = true;

		return signals;
	}

	bool
	StreamContext::is_finished () const
	{
		return finished;
	}


}// Beeps
