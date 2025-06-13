// -*- c++ -*-
#pragma once
#ifndef __BEEPS_SRC_OSX_SIGNALS_H__
#define __BEEPS_SRC_OSX_SIGNALS_H__


#import <AVFoundation/AVFoundation.h>
#include "../signals.h"


namespace Beeps
{


	Signals Signals_create (AVAudioPCMBuffer* buffer, uint capacity = 0);

	void Signals_append (Signals* to, AVAudioPCMBuffer* buffer);


}// Beeps


#endif//EOH
