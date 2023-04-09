// -*- c++ -*-
#pragma once
#ifndef __BEEPS_PROCESSOR_H__
#define __BEEPS_PROCESSOR_H__


#include <xot/ref.h>
#include <xot/pimpl.h>
#include <beeps/defs.h>


namespace Beeps
{


	class Signals;


	class Processor : public Xot::RefCountable<>
	{

		typedef Processor This;

		public:

			class Context {};

			typedef Xot::Ref<This> Ref;

			virtual ~Processor ();

			virtual void reset ();

			virtual void         set_input (Processor* input);

			virtual const Processor* input () const;

			virtual operator bool () const;

			virtual bool operator ! () const;

			struct Data;

			Xot::PImpl<Data> self;

		protected:

			Processor (bool generator = false, float buffering_seconds = 0);

			virtual void process (Context* context, Signals* signals, uint* offset) final;

			virtual void generate (Context* context, Signals* signals, uint* offset);

			virtual void filter (Context* context, Signals* signals, uint* offset);

			virtual void set_updated ();

			friend class ProcessorContext;

	};// Processor


	class Generator : public Processor
	{

		typedef Processor Super;

		protected:

			Generator ();

		private:

			void filter (
				Context* context, Signals* signals, uint* offset) override final;

	};// Generator


	class Filter : public Processor
	{

		typedef Processor Super;

		protected:

			Filter (uint buffering_seconds = 0);

		private:

			void generate (
				Context* context, Signals* signals, uint* offset) override final;

	};// Filter


	class Oscillator : public Generator
	{

		typedef Generator Super;

		public:

			enum Type {NONE = 0, SINE, TRIANGLE, SQUARE, SAWTOOTH};

			Oscillator (Type type = SINE);

			virtual ~Oscillator ();

			virtual void reset () override;

			virtual void set_type (Type type);

			virtual Type     type () const;

			virtual void set_frequency (float frequency);

			virtual float    frequency () const;

			virtual void generate (
				Context* context, Signals* signals, uint* offset) override;

			virtual operator bool () const override;

			struct Data;

			Xot::PImpl<Data> self;

	};// Oscillator


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

			virtual void generate (
				Context* context, Signals* signals, uint* offset) override;

			virtual operator bool () const override;

			struct Data;

			Xot::PImpl<Data> self;

	};// FileIn


	class TimeStretch : public Filter
	{

		typedef Filter Super;

		public:

			TimeStretch (Processor* input = NULL);

			virtual ~TimeStretch ();

			virtual void reset () override;

			virtual void set_scale (float scale);

			virtual float    scale () const;

			virtual void filter (
				Context* context, Signals* signals, uint* offset) override;

			virtual operator bool () const override;

			struct Data;

			Xot::PImpl<Data> self;

	};// TimeStretch


	class PitchShift : public Filter
	{

		typedef Filter Super;

		public:

			PitchShift (Processor* input = NULL);

			virtual ~PitchShift ();

			virtual void reset () override;

			virtual void set_shift (float shift);

			virtual float    shift () const;

			virtual void filter (
				Context* context, Signals* signals, uint* offset) override;

			virtual operator bool () const override;

			struct Data;

			Xot::PImpl<Data> self;

	};// PitchShift


}// Beeps


#endif//EOH
