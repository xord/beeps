#include "beeps/processor.h"


#include "SineWave.h"
#include "BlitSquare.h"
#include "BlitSaw.h"
#include "beeps/exception.h"
#include "signals.h"


namespace Beeps
{


	struct Oscillator::Data
	{

		Type type       = NONE;

		float frequency = 440;

		std::unique_ptr<stk::SineWave> sine;

		std::unique_ptr<stk::BlitSquare> square;

		std::unique_ptr<stk::BlitSaw> saw;

	};// Oscillator::Data


	Oscillator::Oscillator (Type type)
	:	Super(false)
	{
		set_type(type);
	}

	Oscillator::~Oscillator ()
	{
	}

	void
	Oscillator::set_type (Type type)
	{
		if (type == self->type) return;

		self->type = type;

		self->sine  .reset();
		self->square.reset();
		self->saw   .reset();

		switch (self->type)
		{
			case SINE:     self->sine  .reset(new stk::SineWave());   break;
			case SQUARE:   self->square.reset(new stk::BlitSquare()); break;
			case SAWTOOTH: self->saw   .reset(new stk::BlitSaw());    break;
			default:
				argument_error(
					__FILE__, __LINE__, "unknown oscilator type '%d'", self->type);
				break;
		}
	}

	Oscillator::Type
	Oscillator::type () const
	{
		return self->type;
	}

	void
	Oscillator::set_frequency (float frequency)
	{
		if (frequency <= 0)
			argument_error(__FILE__, __LINE__);

		self->frequency = frequency;
	}

	float
	Oscillator::frequency () const
	{
		return self->frequency;
	}

	void
	Oscillator::process (Signals* signals)
	{
		Super::process(signals);

		stk::StkFrames* frames = Signals_get_frames(signals);
		if (!frames)
			invalid_state_error(__FILE__, __LINE__);

		switch (self->type)
		{
			case SINE:
				assert(self->sine);
				self->sine->setFrequency(self->frequency);
				self->sine->tick(*frames);
				break;

			case SQUARE:
				assert(self->square);
				self->square->setFrequency(self->frequency);
				self->square->tick(*frames);
				break;

			case SAWTOOTH:
				assert(self->saw);
				self->saw->setFrequency(self->frequency);
				self->saw->tick(*frames);
				break;

			default:
				invalid_state_error(
					__FILE__, __LINE__, "unknown oscilator type '%d'", self->type);
				break;
		}
	}

	Oscillator::operator bool () const
	{
		if (!Super::operator bool()) return false;
		return
			self->type != NONE && self->frequency > 0 &&
			(self->sine || self->square || self->saw);
	}


}// Beeps
