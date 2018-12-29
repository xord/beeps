// -*- c++ -*-
#pragma once
#ifndef __BEEPS_SRC_SIGNALS_H__
#define __BEEPS_SRC_SIGNALS_H__


#include <Stk.h>
#include <beeps/signals.h>


namespace Beeps
{


	      stk::StkFrames* Signals_get_frames (      Signals* signals);

	const stk::StkFrames* Signals_get_frames (const Signals* signals);


}// Beeps


#endif//EOH
