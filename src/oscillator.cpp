#include "beeps/generator.h"


#include <assert.h>
#include "SineWave.h"
#include "Blit.h"
#include "BlitSquare.h"
#include "BlitSaw.h"
#include "beeps/exception.h"
#include "signals.h"


namespace Beeps
{


	class Osc
	{

		public:

			virtual ~Osc () {}

			virtual void reset () = 0;

			virtual void tick (Frames* frames) = 0;

			virtual void set_frequency (float freq) = 0;

	};// Osc


	template <typename OSC>
	class StkOsc : public Osc
	{

		public:

			void reset () override
			{
				osc.reset();
			}

			void tick (Frames* frames) override
			{
				osc.tick(*frames);
			}

			void set_frequency (float freq) override
			{
				osc.setFrequency(freq);
			}

		protected:

			OSC osc;

	};// StkOsc


	typedef StkOsc<stk::SineWave>   SineOsc;

	typedef StkOsc<stk::BlitSquare> SquareOsc;

	typedef StkOsc<stk::BlitSaw>    SawtoothOsc;


	class TriangleOsc : public StkOsc<stk::Blit>
	{

		public:

			TriangleOsc ()
			{
				osc.setHarmonics(10);
			}

	};// TriangleOsc


	struct Oscillator::Data
	{

		Type type       = TYPE_NONE;

		float frequency = 440;

		std::unique_ptr<Osc> osc;

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
		self->osc->reset();
	}

	void
	Oscillator::set_type (Type type)
	{
		if (type == self->type) return;

		self->type = type;
		self->osc.reset();

		switch (self->type)
		{
			case SINE:     self->osc.reset(new SineOsc());     break;
			case TRIANGLE: self->osc.reset(new TriangleOsc()); break;
			case SQUARE:   self->osc.reset(new SquareOsc());   break;
			case SAWTOOTH: self->osc.reset(new SawtoothOsc()); break;
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

		self->osc->set_frequency(self->frequency);
		self->osc->tick(frames);

		Signals_set_nsamples(signals, frames->nframes());
	}

	Oscillator::operator bool () const
	{
		if (!Super::operator bool()) return false;
		return self->type != TYPE_NONE && self->frequency > 0 && self->osc;
	}


}// Beeps
