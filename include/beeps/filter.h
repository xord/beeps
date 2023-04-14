// -*- c++ -*-
#pragma once
#ifndef __BEEPS_FILTER_H__
#define __BEEPS_FILTER_H__


#include <beeps/processor.h>


namespace Beeps
{


	class Gain : public Filter
	{

		typedef Filter Super;

		public:

			Gain (Processor* input = NULL);

			virtual ~Gain ();

			virtual void set_gain (float gain);

			virtual float    gain () const;

			virtual void filter (
				Context* context, Signals* signals, uint* offset) override;

			struct Data;

			Xot::PImpl<Data> self;

	};// Gain


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
