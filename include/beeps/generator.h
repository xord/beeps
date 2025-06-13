// -*- c++ -*-
#pragma once
#ifndef __BEEPS_GENERATOR_H__
#define __BEEPS_GENERATOR_H__


#include <beeps/processor.h>


namespace Beeps
{


	class Oscillator : public Generator
	{

		typedef Generator Super;

		public:

			enum Type
			{
				TYPE_NONE = 0, SINE, TRIANGLE, SQUARE, SAWTOOTH, NOISE, SAMPLES
			};

			Oscillator (Type type = SINE);

			Oscillator (const Sample* samples, size_t size);

			virtual ~Oscillator ();

			virtual void reset () override;

			virtual void set_type (Type type);

			virtual Type     type () const;

			virtual void      set_samples (const Sample* samples, size_t size);

			virtual const Sample* samples () const;

			virtual size_t       nsamples () const;

			virtual void set_frequency (float      frequency);

			virtual void set_frequency (Processor* frequency);

			virtual float    frequency () const;

			virtual void set_phase (float      phase);

			virtual void set_phase (Processor* phase);

			virtual float    phase () const;

			virtual void set_gain (float      gain);

			virtual void set_gain (Processor* gain);

			virtual float    gain () const;

			virtual void set_offset (float      offset);

			virtual void set_offset (Processor* offset);

			virtual float    offset () const;

			virtual void set_duty (float      duty);

			virtual void set_duty (Processor* duty);

			virtual float    duty () const;

			virtual operator bool () const override;

			struct Data;

			Xot::PImpl<Data> self;

		protected:

			virtual void generate (
				Context* context, Signals* signals, uint* offset) override;

			virtual int max_segment_size_for_process (
				double sample_rate, uint nsamples) const override;

	};// Oscillator


	class Sequencer : public Generator
	{

		typedef Generator Super;

		public:

			Sequencer ();

			virtual ~Sequencer ();

			virtual void add    (Processor* processor, float offset, float duration);

			virtual void remove (Processor* processor, float offset);

			virtual void set_time_scale (float scale);

			virtual float    time_scale () const;

			virtual operator bool () const override;

			struct Data;

			Xot::PImpl<Data> self;

		protected:

			virtual void generate (
				Context* context, Signals* signals, uint* offset) override;

	};// Sequencer


	class FileIn : public Generator
	{

		typedef Generator Super;

		public:

			FileIn (const char* path = NULL);

			virtual ~FileIn ();

			virtual void reset () override;

			virtual void    set_path(const char* path);

			virtual const char* path () const;

			virtual double sample_rate () const;

			virtual uint nchannels () const;

			virtual uint nsamples () const;

			virtual float seconds () const;

			virtual operator bool () const override;

			struct Data;

			Xot::PImpl<Data> self;

		protected:

			virtual void generate (
				Context* context, Signals* signals, uint* offset) override;

	};// FileIn


	class MicIn : public Generator
	{

		typedef Generator Super;

		public:

			MicIn (uint nchannels = 1, double sample_rate = 0);

			virtual ~MicIn ();

			virtual void start ();

			virtual void stop ();

			virtual double sample_rate () const;

			virtual uint nchannels () const;

			virtual operator bool () const override;

			struct Data;

			Xot::PImpl<Data> self;

		protected:

			virtual void generate (
				Context* context, Signals* signals, uint* offset) override;

	};// MicIn


	class TextIn : public Generator
	{

		typedef Generator Super;

		public:

			TextIn (const char* text = NULL, double sample_rate = 0);

			virtual ~TextIn ();

			virtual void synthesize (const char* text);

			virtual double sample_rate () const;

			virtual operator bool () const override;

			struct Data;

			Xot::PImpl<Data> self;

		protected:

			virtual void generate (
				Context* context, Signals* signals, uint* offset) override;

	};// TextIn


}// Beeps


#endif//EOH
