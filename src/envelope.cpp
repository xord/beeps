#include "beeps/filter.h"


#include <assert.h>
#include <ADSR.h>
#include "signals.h"


namespace Beeps
{


	struct Envelope::Data
	{

		stk::ADSR adsr;

		Signals adsr_signals;

		float attack_time = 0, decay_time = 0, sustain_level = 1, release_time = 0;

		float time = 0, note_on_time = -1, note_off_time = -1;

		void update_envelope ()
		{
			adsr.setAttackTime(   attack_time == 0 ? 0.01 :  attack_time);
			adsr.setSustainLevel(sustain_level);
			adsr.setDecayTime(     decay_time == 0 ? 0.01 :   decay_time);
			adsr.setReleaseTime( release_time == 0 ? 0.01 : release_time);
		}

	};// Envelope::Data


	Envelope::Envelope (Processor* input)
	:	Super(input)
	{
		self->update_envelope();
	}

	Envelope::~Envelope ()
	{
	}

	void
	Envelope::note_on (float delay)
	{
		if (delay < 0)
			argument_error(__FILE__, __LINE__);

		float on = self->time + delay, off = self->note_off_time;
		if (0 <= off && off < on)
			self->note_off_time = -1;

		self->note_on_time = on;

		set_updated();
	}

	void
	Envelope::note_off (float delay)
	{
		if (delay < 0)
			argument_error(__FILE__, __LINE__);

		float off = self->time + delay, on = self->note_on_time;
		if (0 <= on && off < on)
			argument_error(__FILE__, __LINE__);

		self->note_off_time = off;

		set_updated();
	}

	void
	Envelope::set_attack_time (float time)
	{
		if (time < 0)
			argument_error(__FILE__, __LINE__);

		self->attack_time = time;
		self->update_envelope();

		set_updated();
	}

	float
	Envelope::attack_time () const
	{
		return self->attack_time;
	}

	void
	Envelope::set_decay_time (float time)
	{
		if (time < 0)
			argument_error(__FILE__, __LINE__);

		self->decay_time = time;
		self->update_envelope();

		set_updated();
	}

	float
	Envelope::decay_time () const
	{
		return self->decay_time;
	}

	void
	Envelope::set_sustain_level (float level)
	{
		if (level < 0)
			argument_error(__FILE__, __LINE__);

		self->sustain_level = level;
		self->update_envelope();

		set_updated();
	}

	float
	Envelope::sustain_level () const
	{
		return self->sustain_level;
	}

	void
	Envelope::set_release_time (float time)
	{
		if (time < 0)
			argument_error(__FILE__, __LINE__);

		self->release_time = time;
		self->update_envelope();

		set_updated();
	}

	float
	Envelope::release_time () const
	{
		return self->release_time;
	}

	static size_t
	slice (Frames* frames, size_t start, float length_sec = -1)
	{
		Float sample_rate = frames->dataRate();
		size_t len        = length_sec >= 0
			?	length_sec * sample_rate
			:	frames->nframes() - start;
		assert(0 < len && (start + len) < frames->nframes());

		return frames->slice(start, len);
	}

	static void
	process_envelope_signals (Envelope* envelope, Signals* signals)
	{
		assert(envelope && signals && signals->nchannels() == 1);

		Envelope::Data* self = envelope->self.get();

		Frames* frames = Signals_get_frames(signals);
		assert(frames);

		float start = self->time;
		float end   = start + Signals_get_seconds(*signals);
		self->time  = end;

		float on  = self->note_on_time;
		float off = self->note_off_time;
		assert(on <= off);

		bool has_on =  0 <= on  && start <= on  && on  < end;
		bool has_off = 0 <= off && start <= off && off < end;

		if (!has_on && !has_off)
		{
			self->adsr.tick(*frames);
			return;
		}

		size_t last = 0;
		if (has_on)
		{
			if (start < on)
			{
				last = slice(frames, 0, on - start);
				self->adsr.tick(*frames);
				frames->unslice();
			}
			self->adsr.keyOn();
		}
		if (has_on || has_off)
		{
			float len = has_off ? off - (has_on ? on : start) : -1;
			last = slice(frames, last, len);
			self->adsr.tick(*frames);
			frames->unslice();
		}
		if (has_off)
		{
			self->adsr.keyOff();
			if (off < end)
			{
				slice(frames, last, -1);
				self->adsr.tick(*frames);
				frames->unslice();
			}
		}
	}

	void
	Envelope::filter (Context* context, Signals* signals, uint* offset)
	{
		Super::filter(context, signals, offset);

		if (!self->adsr_signals)
		{
			self->adsr_signals =
				Signals_create(signals->nsamples(), 1, signals->sample_rate());
		}

		if (self->adsr_signals.nsamples() != signals->nsamples())
			Signals_resize(&self->adsr_signals, signals->nsamples(), 0);

		process_envelope_signals(this, &self->adsr_signals);
		Signals_multiply(signals, self->adsr_signals);
	}

	Envelope::operator bool () const
	{
		if (!Super::operator bool()) return false;
		return
			self->attack_time   >= 0 &&
			self->decay_time    >= 0 &&
			self->sustain_level >= 0 &&
			self->release_time  >= 0 &&
			self->time          >= 0;
	}


}// Beeps
