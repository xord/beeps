// -*- c++ -*-
#pragma once
#ifndef __BEEPS_SOUND_H__
#define __BEEPS_SOUND_H__


#include <xot/pimpl.h>
#include <beeps/defs.h>


namespace Beeps
{


	class Processor;


	class Sound
	{

		public:

			Sound ();

			Sound (Processor* processor, float seconds);

			~Sound ();

			void play ();

			operator bool () const;

			bool operator ! () const;

			struct Data;

			Xot::PSharedImpl<Data> self;

	};// Sound


}// Beeps


#endif//EOH
