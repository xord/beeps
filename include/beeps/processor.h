// -*- c++ -*-
#pragma once
#ifndef __BEEPS_PROCESSOR_H__
#define __BEEPS_PROCESSOR_H__


#include <xot/pimpl.h>
#include <beeps/defs.h>


namespace Beeps
{


	class Signals;


	class Processor
	{

		public:

			virtual ~Processor ();

			virtual void process (Signals* signals);

			virtual operator bool () const;

			virtual bool operator ! () const;

		protected:

			Processor ();

	};// Processor


	class Oscillator : public Processor
	{

		typedef Processor Super;

		public:

			enum Type {NONE = 0, SINE, TRIANGLE, SQUARE, SAWTOOTH};

			Oscillator (Type type = SINE);

			virtual ~Oscillator ();

			virtual void set_type (Type type);

			virtual Type     type () const;

			virtual void set_frequency (float frequency);

			virtual float    frequency () const;

			virtual void process (Signals* signals);

			struct Data;

			Xot::PSharedImpl<Data> self;

	};// Oscillator


	class TimeStretch : public Processor
	{

		typedef Processor Super;

		public:

			TimeStretch ();

			virtual ~TimeStretch ();

			virtual void set_scale (float scale);

			virtual float    scale () const;

			virtual void process (Signals* signals);

			struct Data;

			Xot::PSharedImpl<Data> self;

	};// TimeStretch


	class FileIn : public Processor
	{

		typedef Processor Super;

		public:

			FileIn (const char* path = NULL);

			virtual ~FileIn ();

			virtual void process (Signals* signals);

			virtual uint sampling_rate () const;

			virtual uint nchannels () const;

			virtual float seconds () const;

			virtual operator bool () const;

			struct Data;

			Xot::PSharedImpl<Data> self;

	};// FileIn


}// Beeps


#endif//EOH
