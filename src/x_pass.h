// -*- c++ -*-
#pragma once
#ifndef __BEEPS_SRC_X_PASS_H__
#define __BEEPS_SRC_X_PASS_H__


#include <BiQuad.h>
#include "beeps/filter.h"
#include "signals.h"


namespace Beeps
{


	struct xPassFilterData
	{

		stk::BiQuad biquad;

		float cutoff_freq;

		xPassFilterData (float cutoff_frequency)
		:	cutoff_freq(cutoff_frequency)
		{
		}

		void tick (Signals* signals)
		{
			uint nchannels   = signals->nchannels();
			Signals filtered = Signals_create(
				signals->nsamples(), nchannels, signals->sample_rate());

			Signals_tick(
				&filtered, *signals,
				[&](stk::StkFrames* out, const stk::StkFrames& in)
				{
					for (uint ch = 0; ch < nchannels; ++ch)
						biquad.tick(const_cast<stk::StkFrames&>(in), *out, ch, ch);
				});

			*signals = filtered;
		}

	};// xPassFilterData


}// Beeps


#endif//EOH
