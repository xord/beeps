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

			Signals (float seconds = 1, uint channels = 1);

			~Signals ();

			Signals dup () const;

			float seconds () const;

			uint samples () const;

			uint channels () const;

			operator bool () const;

			bool operator ! () const;

			struct Data;

			Xot::PSharedImpl<Data> self;

	};// Signals


}// Beeps


#endif//EOH
