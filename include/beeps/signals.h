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

			double sample_rate () const;

			uint nchannels () const;

			uint nsamples () const;

			uint capacity () const;

			const double* samples () const;

			operator bool () const;

			bool operator ! () const;

			struct Data;

			Xot::PSharedImpl<Data> self;

	};// Signals


}// Beeps


#endif//EOH
