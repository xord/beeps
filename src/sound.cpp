#include "beeps/sound.h"


#include <limits.h>
#include "Stk.h"
#include "beeps/openal.h"
#include "beeps/signals.h"
#include "beeps/processor.h"
#include "beeps/exception.h"
#include "openal.h"


namespace Beeps
{


	struct Sound::Data
	{

		ALint id;

		Data ()
		:	id(-1)
		{
		}

		~Data ()
		{
			clear();
		}

		void create ()
		{
			if (is_valid()) return;

			ALuint id_ = 0;
			alGenBuffers(1, &id_);
			check_error(__FILE__, __LINE__);

			id = id_;
		}

		void clear ()
		{
			if (id >= 0)
			{
				ALuint id_ = id;
				alDeleteBuffers(1, &id_);
				check_error(__FILE__, __LINE__);
			}

			id = -1;
		}

		bool is_valid () const
		{
			return id >= 0;
		}

	};// Sound::Data


	Sound::Sound ()
	{
	}

	Sound::Sound (Processor* processor, float seconds)
	{
		if (!processor || !*processor || seconds <= 0)
			return;

		self->create();

		Signals signals(seconds, 1);
		processor->process(&signals);

		stk::StkFrames* frames = signals.frames();
		if (!frames)
			return;

		ALsizei size = frames->frames();
		if (size <= 0)
			return;

		std::vector<short> buffer;
		buffer.reserve(size);
		for (ALsizei i = 0; i < size; ++i)
			buffer.push_back((*frames)[i] * SHRT_MAX);

		alBufferData(
			self->id, AL_FORMAT_MONO16, &buffer[0], sizeof(short) * size,
			frames->dataRate());
		check_error(__FILE__, __LINE__);
	}

	Sound::~Sound ()
	{
	}

	void
	Sound::play ()
	{
		play_sound(*this);
	}

	Sound::operator bool () const
	{
		return self->is_valid();
	}

	bool
	Sound::operator ! () const
	{
		return !operator bool();
	}


	ALuint
	get_sound_buffer_id (const Sound& sound)
	{
		return sound.self->id;
	}


}// Beeps
