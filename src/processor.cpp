#include "processor.h"


#include "xot/time.h"
#include "beeps/exception.h"


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


	float
	Processor_get_buffering_seconds (Processor* processor)
	{
		assert(processor);

		return processor->self->buffering_seconds;
	}


	Processor::Processor (bool generator, float buffering_seconds)
	{
		self->generator         = generator;
		self->buffering_seconds = buffering_seconds;
	}

	Processor::~Processor ()
	{
	}

	void
	Processor::reset ()
	{
		if (self->input) self->input->reset();
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
	Processor::process (Signals* signals, uint* offset)
	{
		if (!signals || !*signals)
			argument_error(__FILE__, __LINE__);

		if (self->generator && signals->nsamples() > 0)
			argument_error(__FILE__, __LINE__);

		if (!*this)
			invalid_state_error(__FILE__, __LINE__);
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
	Processor::set_updated ()
	{
		self->last_update_time = Xot::time();
	}


	SignalsBuffer::SignalsBuffer (
		uint nsamples_per_block, uint nchannels, double sample_rate)
	{
		buffer = Signals_create(nsamples_per_block, nchannels, sample_rate);
		assert(*this);
	}

	void
	SignalsBuffer::process (
		Processor* processor, ProcessorContext* context,
		Signals* signals, uint* offset)
	{
		assert(processor && context && signals && offset);

		if (*offset < buffer_offset)
			clear();

		if (buffer.nsamples() == 0)
			buffer_next(processor, context, *offset);

		while (true)
		{
			*offset += Signals_copy(signals, buffer, *offset - buffer_offset);

			bool signals_full = signals->nsamples() == signals->capacity();
			bool  buffer_full =   buffer.nsamples() ==   buffer.capacity();
			if (signals_full || !buffer_full)
				break;

			buffer_next(processor, context, buffer_offset + buffer.nsamples());
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
		Processor* processor, ProcessorContext* context, uint offset)
	{
		Signals_clear(&buffer);
		buffer_offset = offset;

		context->call_process(processor, &buffer, &offset, true);
	}

	void
	SignalsBuffer::clear ()
	{
		Signals_clear(&buffer);
		buffer_offset = 0;
	}


	ProcessorContext::ProcessorContext (
		uint nsamples_per_process, uint nchannels, double sample_rate)
	:	signals(Signals_create(nsamples_per_process, nchannels, sample_rate))
	{
		assert(*this);
	}

	Signals
	ProcessorContext::process (Processor* processor)
	{
		assert(result && processor);

		Signals_clear(&signals);
		call_process(processor, &signals, &offset);

		if (signals.nsamples() < signals.capacity())
			finished = true;

		return signals.nsamples() > 0 ? signals : Signals();
	}

	bool
	ProcessorContext::is_finished () const
	{
		return finished;
	}

	ProcessorContext::operator bool () const
	{
		return signals && !finished;
	}

	bool
	ProcessorContext::operator ! () const
	{
		return !operator bool();
	}

	void
	ProcessorContext::call_process (
		Processor* processor, Signals* signals_, uint* offset_, bool ignore_buffer)
	{
		assert(processor && signals_ && offset_);

		SignalsBuffer* buffer = NULL;
		if (!ignore_buffer && (buffer = get_buffer(processor)))
			buffer->process(processor, this, signals_, offset_);
		else
		{
			if (processor->self->input)
				call_process(processor->self->input, signals_, offset_);

			processor->process(signals_, offset_);
		}
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

		SignalsBuffer* buffer = new SignalsBuffer(
			buffering_sec * signals.sample_rate(),
			signals.nchannels(), signals.sample_rate());

		buffers.emplace(key, buffer);
		return buffer;
	}


}// Beeps
