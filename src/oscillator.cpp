#include "beeps/generator.h"


#include <assert.h>
#include <cmath>
#include <algorithm>
#include <vector>
#include "beeps/exception.h"
#include "beeps/debug.h"
#include "processor.h"
#include "signals.h"


namespace Beeps
{


	static const uint NSEGMENTS_PER_WAVEFORM_CYCLE = 32;

	static const uint MIN_BUFFER_SIZE              = 64;


	class Osc
	{

		public:

			virtual ~Osc () {}

			virtual void reset () = 0;

			virtual void tick (stk::StkFrames* frames) = 0;

			virtual void set_frequency (float freq) = 0;

			virtual float    frequency () const = 0;

			virtual void set_phase (float phase) = 0;

			virtual float    phase () const = 0;

	};// Osc


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

			float frequency () const override
			{
				return freq;
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

			typedef std::vector<Sample> Table;

			WaveformOsc (const Sample* samples, size_t nsamples)
			:	table(samples, samples + nsamples), freq(1), time(0)
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
				Sample prev    = 0;
				for (uint i = 0; i < nframes; ++i, ++p)
				{
					uint ch = i % nchannels;
					if (ch == 0)
					{
						size_t index0 = (size_t) time;
						size_t index1 = index0 == size - 1 ? 0 : index0 + 1;
						float frac    = time - index0;
						*p            = table[index0] * (1.f - frac) + table[index1] * frac;
						prev          = *p;

						time += dt;
						while (time >= size) time -= size;
					}
					else *p = prev;
				}
			}

			void set_frequency (float freq) override
			{
				this->freq = freq;
			}

			float frequency () const override
			{
				return freq;
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

		Type type    = TYPE_NONE;

		float gain   = 1;

		float offset = 0;

		float duty   = 0.5;

		std::unique_ptr<Osc> osc;

	};// Oscillator::Data


	enum InputIndex {FREQUENCY = 0, PHASE, GAIN, OFFSET, DUTY};


	Oscillator::Oscillator (Type type)
	{
		set_type(type);
	}

	Oscillator::Oscillator (const Sample* samples, size_t size)
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

		set_updated();
	}

	static Osc*
	create_osc (Oscillator::Type type, size_t size, float duty)
	{
		std::function<Sample(float)> fun;
		switch (type)
		{
			case Oscillator::SINE:
				fun = [](float t) {return std::sin(t * M_PI * 2);};
				break;

			case Oscillator::TRIANGLE:
				fun = [](float t) {return t < 0.5 ? t * 4 - 1 : 3 - t * 4;};
				break;

			case Oscillator::SAWTOOTH:
				fun = [](float t) {return t * 2 - 1;};
				break;

			case Oscillator::SQUARE:
				fun = [=](float t) {return t < duty ? 1 : -1;};
				break;

			case Oscillator::NOISE:
				return new NoiseOsc();

			default:
				argument_error(
					__FILE__, __LINE__, "unknown oscilator type '%d'", type);
				break;
		}

		std::vector<Sample> samples;
		samples.resize(size);
		for (size_t i = 0; i < size; ++i)
			samples[i] = fun(i / (float) size);

		return new WaveformOsc(&samples[0], samples.size());
	}

	static void
	update_osc (Oscillator* pthis, std::function<Osc*()> fun)
	{
		Oscillator::Data* self = pthis->self.get();

		float freq  = self->osc ? self->osc->frequency() : 440;
		float phase = self->osc ? self->osc->phase()     : 0;

		self->osc.reset(fun());

		self->osc->set_frequency(freq);
		self->osc->set_phase(phase);
	}

	static void
	update_waveform (Oscillator* pthis)
	{
		update_osc(pthis, [&]()
		{
			return create_osc(pthis->type(), 32, pthis->duty());
		});
	}

	void
	Oscillator::set_type (Type type)
	{
		self->type = type;
		update_waveform(this);

		set_updated();
	}

	Oscillator::Type
	Oscillator::type () const
	{
		return self->type;
	}

	void
	Oscillator::set_samples (const Sample* samples, size_t size)
	{
		self->type = SAMPLES;
		update_osc(this, [=]()
		{
			return new WaveformOsc(samples, size);
		});

		set_updated();
	}

	const Sample*
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

		self->osc->set_frequency(frequency);

		set_updated();
		clear_sub_input_unless_processing(FREQUENCY);
	}

	void
	Oscillator::set_frequency (Processor* frequency)
	{
		set_sub_input(FREQUENCY, frequency);
	}

	float
	Oscillator::frequency () const
	{
		return self->osc->frequency();
	}

	void
	Oscillator::set_phase (float phase)
	{
		self->osc->set_phase(phase);

		set_updated();
		clear_sub_input_unless_processing(PHASE);
	}

	void
	Oscillator::set_phase (Processor* phase)
	{
		set_sub_input(PHASE, phase);
	}

	float
	Oscillator::phase () const
	{
		return self->osc->phase();
	}

	void
	Oscillator::set_gain (float gain)
	{
		if (gain == self->gain)
			return;

		self->gain = gain;

		set_updated();
		clear_sub_input_unless_processing(GAIN);
	}

	void
	Oscillator::set_gain (Processor* gain)
	{
		set_sub_input(GAIN, gain);
	}

	float
	Oscillator::gain () const
	{
		return self->gain;
	}

	void
	Oscillator::set_offset (float offset)
	{
		if (offset == self->offset)
			return;

		self->offset = offset;

		set_updated();
		clear_sub_input_unless_processing(OFFSET);
	}

	void
	Oscillator::set_offset (Processor* offset)
	{
		set_sub_input(OFFSET, offset);
	}

	float
	Oscillator::offset () const
	{
		return self->offset;
	}

	void
	Oscillator::set_duty (float duty)
	{
		if (duty <= 0)
			argument_error(__FILE__, __LINE__);
		if (duty >= 1)
			argument_error(__FILE__, __LINE__);

		if (duty == self->duty)
			return;

		self->duty = duty;
		if (type() == SQUARE) update_waveform(this);

		set_updated();
		clear_sub_input_unless_processing(DUTY);
	}

	void
	Oscillator::set_duty (Processor* duty)
	{
		set_sub_input(DUTY, duty);
	}

	float
	Oscillator::duty () const
	{
		return self->duty;
	}

	Oscillator::operator bool () const
	{
		if (!Super::operator bool())
			return false;

		return self->type != TYPE_NONE && self->osc;
	}

	void
	Oscillator::generate (Context* context, Signals* signals, uint* offset)
	{
		Super::generate(context, signals, offset);

		Processor* pfreq   = sub_input(FREQUENCY);
		Processor* pphase  = sub_input(PHASE);
		Processor* pgain   = sub_input(GAIN);
		Processor* poffset = sub_input(OFFSET);
		Processor* pduty   = sub_input(DUTY);

		auto* pcontext     = Processor_get_context(context);
		double sample_rate = signals->sample_rate();
		uint nsamples      = signals->capacity();
		uint seg_size      = get_segment_size(sample_rate, nsamples);

		for (uint start = 0; start < nsamples; start += seg_size)
		{
			uint seg_offset = *offset + start;
			if (pfreq)   set_frequency(pcontext->process(pfreq,   seg_size, seg_offset));
			if (pphase)  set_phase(    pcontext->process(pphase,  seg_size, seg_offset));
			if (pgain)   set_gain(     pcontext->process(pgain,   seg_size, seg_offset));
			if (poffset) set_offset(   pcontext->process(poffset, seg_size, seg_offset));
			if (pduty)   set_duty(     pcontext->process(pduty,   seg_size, seg_offset));

			uint end = std::min(start + seg_size, nsamples);
			Signals_tick(signals, start, end, [&](stk::StkFrames* frames)
			{
				self->osc->tick(frames);
			});
			Signals_offset_and_scale(signals, self->offset, self->gain, -1, start, end);
		}

		*offset += signals->nsamples();
	}

	int
	Oscillator::max_segment_size_for_process (
		double sample_rate, uint nsamples) const
	{
		return std::clamp(
			(uint) (sample_rate / frequency() / NSEGMENTS_PER_WAVEFORM_CYCLE),
			MIN_BUFFER_SIZE,
			nsamples);
	}


}// Beeps
