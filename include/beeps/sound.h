// -*- c++ -*-
#pragma once
#ifndef __BEEPS_SOUND_H__
#define __BEEPS_SOUND_H__


#include <xot/pimpl.h>
#include <beeps/defs.h>


namespace Beeps
{


	class Processor;


	class SoundPlayer
	{

		public:

			SoundPlayer ();

			~SoundPlayer ();

			void play ();

			void pause ();

			void rewind ();

			void stop ();

			bool is_playing () const;

			bool is_stopped () const;

			void set_gain (float gain);

			float    gain () const;

			void set_loop (bool loop);

			bool     loop () const;

			operator bool () const;

			bool operator ! () const;

			struct Data;

			Xot::PSharedImpl<Data> self;

	};// SoundPlayer


	class Sound
	{

		public:

			Sound ();

			Sound (
				Processor* processor, float seconds,
				uint nchannels = 1, double sample_rate = 0);

			~Sound ();

			SoundPlayer play ();

			void save (const char* path) const;

			double sample_rate () const;

			uint nchannels () const;

			float seconds () const;

			operator bool () const;

			bool operator ! () const;

			struct Data;

			Xot::PSharedImpl<Data> self;

	};// Sound


	Sound load_sound (const char* path);


}// Beeps


#endif//EOH
