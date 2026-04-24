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

			Signals (uint capacity, uint nchannels = 1, double sample_rate = 0);

			Signals (
				const float* const* channels, uint nsamples, uint nchannels,
				double sample_rate = 0, uint capacity = 0);

			~Signals ();

			Signals dup () const;

			void clear (uint capacity = 0);

			uint append (
				const float* const* channels, uint nsamples, uint nchannels,
				double sample_rate = 0);

			uint append (const Signals& source, uint source_offset = 0);

			double sample_rate () const;

			uint nchannels () const;

			uint nsamples () const;

			uint capacity () const;

			bool empty () const;

			bool full() const;

			const Sample* samples () const;

			operator bool () const;

			bool operator ! () const;

			struct Data;

			Xot::PSharedImpl<Data> self;

	};// Signals


}// Beeps


#endif//EOH
