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

			typedef Xot::Ref<This> Ref;

			virtual ~Processor ();

			virtual void reset ();

			virtual void         set_input (Processor* input);

			virtual const Processor* input () const;

			virtual void process (Signals* signals, uint* offset) = 0;

			virtual operator bool () const;

			virtual bool operator ! () const;

			struct Data;

			Xot::PImpl<Data> self;

		protected:

			Processor (bool generator = false);

	};// Processor


	class Oscillator : public Processor
	{

		typedef Processor Super;

		public:

			enum Type {NONE = 0, SINE, TRIANGLE, SQUARE, SAWTOOTH};

			Oscillator (Type type = SINE);

			virtual ~Oscillator ();

			virtual void reset ();

			virtual void set_type (Type type);

			virtual Type     type () const;

			virtual void set_frequency (float frequency);

			virtual float    frequency () const;

			virtual void process (Signals* signals, uint* offset);

			virtual operator bool () const;

			struct Data;

			Xot::PImpl<Data> self;

	};// Oscillator


	class FileIn : public Processor
	{

		typedef Processor Super;

		public:

			FileIn (const char* path = NULL);

			virtual ~FileIn ();

			virtual void reset ();

			virtual void    set_path(const char* path);

			virtual const char* path () const;

			virtual uint sample_rate () const;

			virtual uint nchannels () const;

			virtual uint nsamples () const;

			virtual float seconds () const;

			virtual void process (Signals* signals, uint* offset);

			virtual operator bool () const;

			struct Data;

			Xot::PImpl<Data> self;

	};// FileIn


	class TimeStretch : public Processor
	{

		typedef Processor Super;

		public:

			TimeStretch (Processor* input = NULL);

			virtual ~TimeStretch ();

			virtual void reset ();

			virtual void set_scale (float scale);

			virtual float    scale () const;

			virtual void process (Signals* signals, uint* offset);

			virtual operator bool () const;

			struct Data;

			Xot::PImpl<Data> self;

	};// TimeStretch


	class PitchShift : public Processor
	{

		typedef Processor Super;

		public:

			PitchShift (Processor* input = NULL);

			virtual ~PitchShift ();

			virtual void reset ();

			virtual void set_shift (float shift);

			virtual float    shift () const;

			virtual void process (Signals* signals, uint* offset);

			virtual operator bool () const;

			struct Data;

			Xot::PImpl<Data> self;

	};// PitchShift


}// Beeps


#endif//EOH
