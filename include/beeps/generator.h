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

			enum Type {TYPE_NONE = 0, SINE, TRIANGLE, SQUARE, SAWTOOTH};

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

			virtual void generate (
				Context* context, Signals* signals, uint* offset) override;

			virtual operator bool () const override;

			struct Data;

			Xot::PImpl<Data> self;

	};// MicIn


}// Beeps


#endif//EOH
