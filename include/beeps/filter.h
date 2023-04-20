// -*- c++ -*-
#pragma once
#ifndef __BEEPS_FILTER_H__
#define __BEEPS_FILTER_H__


#include <vector>
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


	class ADSR : public Filter
	{

		typedef Filter Super;

		public:

			ADSR (Processor* input = NULL);

			virtual ~ADSR ();

			virtual void note_on  (float delay = 0);

			virtual void note_off (float delay = 0);

			virtual void set_attack_time (float time);

			virtual float    attack_time () const;

			virtual void set_decay_time (float time);

			virtual float    decay_time () const;

			virtual void set_sustain_level (float level);

			virtual float    sustain_level () const;

			virtual void set_release_time (float time);

			virtual float    release_time () const;

			virtual void filter (
				Context* context, Signals* signals, uint* offset) override;

			virtual operator bool () const override;

			struct Data;

			Xot::PImpl<Data> self;

	};// ADSR


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


	class Analyser : public Filter
	{

		typedef Filter Super;

		public:

			typedef std::vector<float> Spectrum;

			Analyser (uint fft_size = 1024, Processor* input = NULL);

			virtual ~Analyser ();

			virtual void set_fft_size (uint size);

			virtual uint     fft_size () const;

			virtual float resolution () const;

			virtual const Signals&  signals () const;

			virtual const Spectrum& spectrum () const;

			virtual void filter (
				Context* context, Signals* signals, uint* offset) override;

			virtual operator bool () const override;

			struct Data;

			Xot::PImpl<Data> self;

	};// Analyser


}// Beeps


#endif//EOH
