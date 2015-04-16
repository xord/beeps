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

			virtual void process (Signals* signals);

			struct Data;

			Xot::PImpl<Data, true> self;

	};// SineWave


	class FileInput : public Processor
	{

		typedef Processor Super;

		public:

			FileInput (const char* path = NULL);

			virtual ~FileInput ();

			virtual void process (Signals* signals);

			virtual operator bool () const;

			struct Data;

			Xot::PImpl<Data, true> self;

	};// FileInput


}// Beeps


#endif//EOH
