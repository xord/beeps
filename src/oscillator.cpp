#include "beeps/generator.h"


#include <assert.h>
#include <cmath>
#include <vector>
#include "SineWave.h"
#include "Blit.h"
#include "BlitSquare.h"
#include "BlitSaw.h"
#include "beeps/exception.h"
#include "beeps/debug.h"
#include "signals.h"


namespace Beeps
{


	class SineWave : public stk::SineWave
	{

		public:

			void setPhase (stk::StkFloat phase)
			{
				time_ = std::fmod(phase, 1.f) * TABLE_SIZE;
			}

			stk::StkFloat getPhase () const
			{
				return time_ / TABLE_SIZE;
			}

	};// SineWave


	class BlitSaw : public stk::BlitSaw
	{

		public:

			void setPhase (stk::StkFloat phase)
			{
				phase_ = M_PI * phase;
			}

			stk::StkFloat getPhase () const
			{
				return phase_ / M_PI;
			}

	};// BlitSaw


	class Osc
	{

		public:

			virtual ~Osc () {}

			virtual void reset () = 0;

			virtual void tick (stk::StkFrames* frames) = 0;

			virtual void set_frequency (float freq) = 0;

			virtual void set_phase (float phase) = 0;

			virtual float    phase () const = 0;

	};// Osc


	template <typename OSC, uint DROP_MSEC>
	class StkOsc : public Osc
	{

		public:

			void reset () override
			{
				osc.reset();
				drop_msec = DROP_MSEC;
			}

			void tick (stk::StkFrames* frames) override
			{
				if (drop_msec > 0)
				{
					stk::StkFrames f((uint) (frames->dataRate() * (drop_msec / 1000.0)), 1);
					osc.tick(f);
					drop_msec = 0;
				}

				osc.tick(*frames);
			}

			void set_frequency (float freq) override
			{
				osc.setFrequency(freq);
			}

			void set_phase (float phase) override
			{
				osc.setPhase(phase);
			}

			float phase () const override
			{
				return osc.getPhase();
			}

		protected:

			OSC osc;

			uint drop_msec = DROP_MSEC;

	};// StkOsc


	typedef StkOsc<SineWave,          0> SineOsc;

	typedef StkOsc<stk::BlitSquare, 100> SquareOsc;

	typedef StkOsc<BlitSaw,         200> SawtoothOsc;


	class TriangleOsc : public StkOsc<stk::Blit, 0>
	{

		public:

			TriangleOsc ()
			{
				osc.setHarmonics(10);
			}

	};// TriangleOsc


	class NoiseOsc : public Osc
	{

		public:

			void reset () override
			{
				time = 0;
			}

			void tick (stk::StkFrames* frames) override
			{
				if (0 < freq && (freq * 2) <= frames->dataRate())
					tick_with_freq(frames);
				else
					tick_without_freq(frames);
			}

			void set_frequency (float freq) override
			{
				this->freq = freq;
			}

			void set_phase (float phase) override
			{
				time = freq == 0 ? 0 : phase * (1.f / freq);
			}

			float phase () const override
			{
				return freq == 0 ? 0 : time / (1.f / freq);
			}

		private:

			float freq  = 0;

			double time = 0;

			void tick_without_freq (stk::StkFrames* frames)
			{
				uint nchannels = frames->channels();
				uint nframes   = frames->frames();
				Sample* p      = &(*frames)(0, 0);
				for (uint i = 0; i < nframes; ++i, ++p)
				{
					uint ch = i % nchannels;
					*p      = ch == 0 ? noise() : p[-ch];
				}
			}

			void tick_with_freq (stk::StkFrames* frames)
			{
				float time_per_sample = 1.f / frames->dataRate();
				float time_per_freq   = 1.f / (freq * 2);

				uint nchannels = frames->channels();
				uint nframes   = frames->frames();
				Sample* p      = &(*frames)(0, 0);
				float value    = noise();
				for (uint i = 0; i < nframes; ++i, ++p)
				{
					if (i % nchannels == 0)
					{
						*p = value;

						time += time_per_sample;
						while (time >= time_per_freq)
						{
							value = noise();
							time -= time_per_freq;
						}
					}
					else
						*p = value;
				}
			}

			double noise () const
			{
				return rand() / ((double) RAND_MAX + 1) * 2 - 1;
			}

	};// NoiseOsc


	class WaveformOsc : public Osc
	{

		public:

			typedef std::vector<float> Table;

			WaveformOsc (float* samples, size_t nsamples, float frequency)
			:	table(samples, samples + nsamples), freq(frequency), time(0)
			{
			}

			void reset () override
			{
				time = 0;
			}

			void tick (stk::StkFrames* frames) override
			{
				size_t size = table.size();
				float dt    = size * freq / frames->dataRate();

				uint nchannels = frames->channels();
				uint nframes   = frames->frames();
				Sample* p      = &(*frames)(0, 0);
				for (uint i = 0; i < nframes; ++i, ++p)
				{
					uint ch = i % nchannels;
					if (ch == 0)
					{
						size_t index0 = (size_t) time;
						size_t index1 = index0 == size - 1 ? 0 : index0 + 1;
						float frac    = time - index0;
						*p            = table[index0] * (1.f - frac) + table[index1] * frac;

						time += dt;
						while (time >= size) time -= size;
					}
					else
						*p = p[-ch];
				}
			}

			void set_frequency (float freq) override
			{
				this->freq = freq;
			}

			void set_phase (float phase) override
			{
				this->time = std::fmod(phase, 1.f) * table.size();
			}

			float phase () const override
			{
				return time / table.size();
			}

			const Table& samples () const
			{
				return table;
			}

		private:

			Table table;

			float freq, time;

	};// WaveformOsc


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

	Oscillator::Oscillator (float* samples, size_t size)
	{
		set_samples(samples, size);
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

		float phase = self->osc ? self->osc->phase() : 0;

		self->type = type;
		self->osc.reset();

		switch (self->type)
		{
			case SINE:     self->osc.reset(new SineOsc());     break;
			case TRIANGLE: self->osc.reset(new TriangleOsc()); break;
			case SQUARE:   self->osc.reset(new SquareOsc());   break;
			case SAWTOOTH: self->osc.reset(new SawtoothOsc()); break;
			case NOISE:    self->osc.reset(new NoiseOsc());    break;
			default:
				argument_error(
					__FILE__, __LINE__, "unknown oscilator type '%d'", self->type);
				break;
		}

		self->osc->set_phase(phase);

		set_updated();
	}

	Oscillator::Type
	Oscillator::type () const
	{
		return self->type;
	}

	void
	Oscillator::set_samples (float* samples, size_t size)
	{
		float phase = self->osc ? self->osc->phase() : 0;

		self->type = SAMPLES;
		self->osc.reset(new WaveformOsc(samples, size, frequency()));
		self->osc->set_phase(phase);

		set_updated();
	}

	const float*
	Oscillator::samples () const
	{
		if (self->type != SAMPLES)
			return NULL;

		auto* osc = (const WaveformOsc*) self->osc.get();
		return &osc->samples()[0];
	}

	size_t
	Oscillator::nsamples () const
	{
		if (self->type != SAMPLES)
			return 0;

		auto* osc = (WaveformOsc*) self->osc.get();
		return osc->samples().size();
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
	Oscillator::set_phase (float phase)
	{
		self->osc->set_phase(phase);
		set_updated();
	}

	float
	Oscillator::phase () const
	{
		return self->osc->phase();
	}

	void
	Oscillator::generate (Context* context, Signals* signals, uint* offset)
	{
		Super::generate(context, signals, offset);

		self->osc->set_frequency(self->frequency);
		Signals_tick(signals, [&](stk::StkFrames* frames)
		{
			self->osc->tick(frames);
		});

		*offset += signals->nsamples();
	}

	Oscillator::operator bool () const
	{
		if (!Super::operator bool()) return false;
		return self->type != TYPE_NONE && self->frequency > 0 && self->osc;
	}


}// Beeps
