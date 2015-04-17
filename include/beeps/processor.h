// -*- c++ -*-
#pragma once
#ifndef __BEEPS_PROCESSOR_H__
#define __BEEPS_PROCESSOR_H__


#include <xot/pimpl.h>


namespace Beeps
{


	class Signals;


	class Processor
	{

		public:

			Processor ();

			virtual ~Processor ();

			virtual void process (Signals* signals);

			virtual operator bool () const;

			virtual bool operator ! () const;

	};// Processor


	class SineWave : public Processor
	{

		typedef Processor Super;

		public:

			SineWave ();

			virtual ~SineWave ();

			virtual void set_frequency (float frequency);

			virtual float    frequency () const;

			virtual void process (Signals* signals);

			struct Data;

			Xot::PImpl<Data, true> self;

	};// SineWave


	class FileIn : public Processor
	{

		typedef Processor Super;

		public:

			FileIn (const char* path = NULL);

			virtual ~FileIn ();

			virtual void process (Signals* signals);

			virtual operator bool () const;

			struct Data;

			Xot::PImpl<Data, true> self;

	};// FileIn


}// Beeps


#endif//EOH
