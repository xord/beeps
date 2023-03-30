// -*- c++ -*-
#pragma once
#ifndef __BEEPS_SIGNALS_H__
#define __BEEPS_SIGNALS_H__


#include <xot/pimpl.h>
#include <beeps/defs.h>


namespace Beeps
{


	class Signals
	{

		public:

			Signals ();

			~Signals ();

			Signals dup () const;

			void clear ();

			uint sample_rate () const;

			uint nsamples () const;

			uint nchannels () const;

			float seconds () const;

			operator bool () const;

			bool operator ! () const;

			struct Data;

			Xot::PSharedImpl<Data> self;

	};// Signals


}// Beeps


#endif//EOH
