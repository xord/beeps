// -*- c++ -*-
#pragma once
#ifndef __BEEPS_SRC_SIGNALS_H__
#define __BEEPS_SRC_SIGNALS_H__


#include <Stk.h>
#include <beeps/signals.h>


namespace Beeps
{


	Signals Signals_create (
		float seconds, uint nchannels = 1, uint sampling_rate = 0);

	Signals Signals_create (
		const float* const* channels,
		uint nsamples, uint nchannels, uint sampling_rate = 0);

	bool Signals_copy (
		Signals* to, const Signals& from, float from_offset_sec = 0);

	      stk::StkFrames* Signals_get_frames (      Signals* signals);

	const stk::StkFrames* Signals_get_frames (const Signals* signals);


}// Beeps


#endif//EOH
