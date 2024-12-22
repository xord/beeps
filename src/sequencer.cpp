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

		float to_time (const Signals& signals, uint nsamples) const
		{
			return nsamples / signals.sample_rate() * time_scale;
		}

		uint from_time (const Signals& signals, float time) const
		{
			return time * signals.sample_rate() / time_scale;
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
		Frames* mixf = Signals_get_frames(mixed);
		Frames* srcf = Signals_get_frames(const_cast<Signals*>(&source));
		Float* mixp  = &(*mixf)(source_offset, 0);
		Float* srcp  = &(*srcf)(0, 0);
		uint size    = source.nsamples() * source.nchannels();
		for (uint i = 0; i < size; ++i, ++mixp, ++srcp)
			*mixp += *srcp;
	}

	void
	Sequencer::generate (Context* pcontext, Signals* psignals, uint* offset)
	{
		Super::generate(pcontext, psignals, offset);

		auto& context = *Processor_get_context(pcontext);
		auto& signals = *psignals;
		Signals_resize(&signals, signals.capacity(), 0);

		float generate_begin = self->to_time(signals, *offset);
		float generate_end   = self->to_time(signals, *offset + signals.capacity());
		Signals note_signals = Signals_create(
			signals.capacity(), signals.nchannels(), signals.sample_rate());

		uint nsamples = 0;
		for (auto& note : self->notes)
		{
			float note_begin = note.offset;
			float note_end   = note.offset + note.duration;
			if (note_end < generate_begin) continue;
			if (generate_end < note_begin) break;

			float begin = std::max(generate_begin, note_begin);
			float end   = std::min(generate_end,   note_end);
			if (begin == end) continue;

			uint note_offset = self->from_time(signals, begin - note_begin);
			Signals_clear(&note_signals, self->from_time(signals, end - begin));
			context.process(note.processor.get(), &note_signals, &note_offset);

			uint note_begin_offset =
				self->from_time(signals, note_begin - generate_begin);
			mix(&signals, note_signals, note_begin_offset);

			if (note_begin_offset + note_signals.nsamples() > nsamples)
				nsamples = note_begin_offset + note_signals.nsamples();
		}

		Signals_set_nsamples(&signals, nsamples);
	}

	Sequencer::operator bool () const
	{
		if (!Super::operator bool()) return false;
		return self->time_scale > 0;
	}


}// Beeps
