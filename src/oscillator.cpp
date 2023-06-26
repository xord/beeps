#include "beeps/generator.h"


#include "SineWave.h"
#include "BlitSquare.h"
#include "BlitSaw.h"
#include "beeps/exception.h"
#include "signals.h"


namespace Beeps
{


	struct Oscillator::Data
	{

		Type type       = TYPE_NONE;

		float frequency = 440;

		std::unique_ptr<stk::SineWave> sine;

		std::unique_ptr<stk::BlitSquare> square;

		std::unique_ptr<stk::BlitSaw> saw;

	};// Oscillator::Data


	Oscillator::Oscillator (Type type)
	{
		set_type(type);
	}

	Oscillator::~Oscillator ()
	{
	}

	void
	Oscillator::reset ()
	{
		Super::reset();

		if (self->sine)   self->sine->reset();
		if (self->square) self->square->reset();
		if (self->saw)    self->saw->reset();
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

		set_updated();
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

		set_updated();
	}

	float
	Oscillator::frequency () const
	{
		return self->frequency;
	}

	void
	Oscillator::generate (Context* context, Signals* signals, uint* offset)
	{
		Super::generate(context, signals, offset);

		Frames* frames = Signals_get_frames(signals);
		if (!frames)
			argument_error(__FILE__, __LINE__);

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

		Signals_set_nsamples(signals, frames->nframes());
	}

	Oscillator::operator bool () const
	{
		if (!Super::operator bool()) return false;
		return
			self->type != TYPE_NONE && self->frequency > 0 &&
			(self->sine || self->square || self->saw);
	}


}// Beeps
