#include "beeps/generator.h"


#include <list>
#include <algorithm>
#include "beeps/exception.h"
#include "beeps/debug.h"
#include "processor.h"
#include "signals.h"


namespace Beeps
{


	struct Sequencer::Data
	{

		struct Note
		{

			Processor::Ref processor;

			float offset, duration;

			Note (Processor* processor, float offset, float duration)
			:	processor(processor), offset(offset), duration(duration)
			{
			}

		};// Note

		std::list<Note> notes;

		float time_scale = 1;

		uint sec2nsample (const Signals& signals, float sec) const
		{
			return sec * signals.sample_rate() / time_scale;
		}

	};// Sequencer::Data


	Sequencer::Sequencer ()
	{
	}

	Sequencer::~Sequencer ()
	{
	}

	void
	Sequencer::add (Processor* processor, float offset, float duration)
	{
		auto& notes = self->notes;
		for (auto it = notes.begin(), end = notes.end(); it != end; ++it)
		{
			if (offset < it->offset)
			{
				notes.emplace(it, processor, offset, duration);
				return;
			}
		}
		notes.emplace_back(processor, offset, duration);
	}

	void
	Sequencer::remove (Processor* processor, float offset)
	{
		auto it =
			std::find_if(self->notes.begin(), self->notes.end(), [&](auto& note)
			{
				return note.processor == processor && note.offset == offset;
			});
		if (it == self->notes.end()) return;

		self->notes.erase(it);
	}

	void
	Sequencer::set_time_scale (float time_scale)
	{
		if (time_scale <= 0)
			argument_error(__FILE__, __LINE__, "time_scale must be greater than 0");

		self->time_scale = time_scale;
	}

	float
	Sequencer::time_scale () const
	{
		return self->time_scale;
	}

	static void
	mix (Signals* mixed, const Signals& source, uint source_offset)
	{
		      Sample* mix_p = Signals_at(mixed,  source_offset);
		const Sample* src_p = Signals_at(source, 0);
		uint size     = source.nsamples() * source.nchannels();
		for (uint i = 0; i < size; ++i, ++mix_p, ++src_p)
			*mix_p += *src_p;
	}

	void
	Sequencer::generate (Context* pcontext, Signals* psignals, uint* offset)
	{
		Super::generate(pcontext, psignals, offset);

		auto& context = *Processor_get_context(pcontext);
		auto& signals = *psignals;
		Signals_fill(&signals, signals.capacity(), 0);

		uint generate_begin  = *offset;
		uint generate_end    = *offset + signals.capacity();
		Signals note_signals = Signals_create(
			signals.capacity(), signals.nchannels(), signals.sample_rate());

		uint nsamples = 0;
		for (auto& note : self->notes)
		{
			uint note_begin = self->sec2nsample(signals, note.offset);
			uint note_end   = self->sec2nsample(signals, note.offset + note.duration);
			if (note_end <= generate_begin) continue;
			if (generate_end <= note_begin)
			{
				nsamples = generate_end - generate_begin;
				break;
			}

			uint begin = std::max(generate_begin, note_begin);
			uint end   = std::min(generate_end,   note_end);
			assert(begin != end);

			uint note_offset = begin - note_begin;
			Signals_clear(&note_signals, end - begin);
			context.process(note.processor.get(), &note_signals, &note_offset);

			uint mix_offset  = begin - generate_begin;
			mix(&signals, note_signals, mix_offset);

			if (mix_offset + note_signals.nsamples() > nsamples)
				nsamples = mix_offset + note_signals.nsamples();
		}

		Signals_set_nsamples(&signals, nsamples);
		*offset += nsamples;
	}

	Sequencer::operator bool () const
	{
		if (!Super::operator bool()) return false;
		return self->time_scale > 0;
	}


}// Beeps
