#include "beeps/filter.h"


#include <assert.h>
#include <ADSR.h>
#include "beeps/debug.h"
#include "signals.h"


namespace Beeps
{


	static const float TIME_ZERO = 0.0000001;


	class ADSR : public stk::ADSR
	{

		public:

			void skipAttackPhase()
			{
				value_  = 1;
				target_ = 1;
				state_  = DECAY;
			}

	};// ADSR


	struct Envelope::Data
	{

		ADSR adsr;

		Signals adsr_signals;

		float attack_time   = 0.005;
		float decay_time    = 0.005;
		float sustain_level = 1;
		float release_time  = 0.005;

		float time = 0, note_on_time = -1, note_off_time = -1;

		void update_envelope ()
		{
			adsr.setSustainLevel(sustain_level);
			adsr.setAttackTime(  attack_time == 0 ? TIME_ZERO :  attack_time);
			adsr.setDecayTime(    decay_time == 0 ? TIME_ZERO :   decay_time);
			adsr.setReleaseTime(release_time == 0 ? TIME_ZERO : release_time);
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

	static uint
	tick (Envelope* envelope, Signals* signals, uint start, float length_sec = -1)
	{
		uint max = signals->capacity() - start;
		uint len = length_sec >= 0 ? length_sec * signals->sample_rate() : max;
		if (len > max) len = max;
		assert(0 < len && (start + len) <= signals->nsamples());

		return Signals_tick(
			signals, start, start + len,
			[&](stk::StkFrames* frames) {envelope->self->adsr.tick(*frames);});
	}

	static void
	process_envelope_signals (Envelope* envelope, Signals* signals)
	{
		assert(envelope && signals && signals->nchannels() == 1);

		Envelope::Data* self = envelope->self.get();

		if (self->time == 0 && self->attack_time == 0)
			self->adsr.skipAttackPhase();

		float on          = self->note_on_time;
		float off         = self->note_off_time;
		float start       = self->time;
		float end         = start + signals->capacity() / signals->sample_rate();
		float release_end = off >= 0 ? off + self->release_time : -1;
		bool has_on       = 0 <= on  && start <= on  && on  < end;
		bool has_off      = 0 <= off && start <= off && off < end;

		if (release_end >= 0 && release_end < end)
			end = release_end;

		self->time = end;

		if (!has_on && !has_off)
		{
			float len = end == release_end ? end - start : -1;
			tick(envelope, signals, 0, len);
			return;
		}

		uint pos = 0;
		if (has_on)
		{
			if (start < on)
				pos = tick(envelope, signals, pos, on - start);
			self->adsr.keyOn();
		}
		if (has_on || has_off)
		{
			float len = has_off ? off - (has_on ? on : start) : -1;
			pos = tick(envelope, signals, pos, len);
		}
		if (has_off)
		{
			self->adsr.keyOff();
			if (off < end)
			{
				float len = end == release_end ? end - off : -1;
				pos = tick(envelope, signals, pos, len);
			}
		}

		Signals_set_nsamples(signals, pos);
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
		else
			Signals_clear(&self->adsr_signals, signals->nsamples());

		process_envelope_signals(this, &self->adsr_signals);
		if (self->adsr_signals.nsamples() < signals->nsamples())
			Signals_set_nsamples(signals, self->adsr_signals.nsamples());

		Signals_multiply(signals, self->adsr_signals);
	}


}// Beeps
