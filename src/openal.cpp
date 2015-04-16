#include "beeps/openal.h"


#include <vector>
#include "beeps/defs.h"
#include "beeps/sound.h"
#include "beeps/exception.h"
#include "openal.h"


namespace Beeps
{


	ALCdevice* get_device ();


	ALCenum
	get_error ()
	{
		return alcGetError(get_device());
	}

	bool
	no_error ()
	{
		return get_error() == ALC_NO_ERROR;
	}

	bool
	is_error (ALCenum err)
	{
		return get_error() == err;
	}

	static String
	get_error_name (ALenum error)
	{
		switch (error)
		{
			case ALC_NO_ERROR:        return "ALC_NO_ERROR";
			case ALC_INVALID_DEVICE:  return "ALC_INVALID_DEVICE";
			case ALC_INVALID_CONTEXT: return "ALC_INVALID_CONTEXT";
			case ALC_INVALID_ENUM:    return "ALC_INVALID_ENUM";
			case ALC_OUT_OF_MEMORY:   return "ALC_OUT_OF_MEMORY";
		}
		return "UNKNOWN ERROR";
	}

	void
	check_error (const char* file, int line)
	{
		ALCenum e = get_error();
		if (e != ALC_NO_ERROR)
			openal_error(file, line, "OpenAL error %s", get_error_name(e).c_str());
	}

	void
	clear_error ()
	{
		get_error();
	}


	struct SoundSource
	{

		typedef SoundSource This;

		typedef boost::shared_ptr<This> Ptr;

		ALint id;

		static Ptr create ()
		{
			ALuint id_;
			alGenSources(1, &id_);
			if (!no_error()) return Ptr();

			This* p = new This;
			p->id = id_;
			return Ptr(p);
		}

		SoundSource ()
		:	id(-1)
		{
		}

		~SoundSource ()
		{
			if (!*this) return;

			ALuint id_ = id;
			alDeleteSources(1, &id_);
			check_error(__FILE__, __LINE__);
		}

		bool is_playing () const
		{
			if (!*this) return false;

			ALint state = 0;
			alGetSourcei(id, AL_SOURCE_STATE, &state);
			check_error(__FILE__, __LINE__);

			return state == AL_PLAYING;
		}

		operator bool () const
		{
			return id >= 0;
		}

		bool operator ! () const
		{
			return !operator bool();
		}

	};// SoundSource


	typedef std::vector<SoundSource::Ptr> SoundSourceList;

	static SoundSourceList sources;


	static SoundSource*
	next_source ()
	{
		for (SoundSourceList::iterator it = sources.begin(); it != sources.end(); ++it)
		{
			const SoundSource::Ptr& p = *it;
			if (!p || !*p || p->is_playing())
				continue;

			sources.erase(it);
			sources.push_back(p);
			return p.get();
		}

		SoundSource::Ptr new_source = SoundSource::create();
		if (new_source)
		{
			sources.push_back(new_source);
			return new_source.get();
		}

		SoundSource::Ptr oldest = *sources.begin();
		if (oldest)
		{
			alSourceStop(oldest->id);
			check_error(__FILE__, __LINE__);

			sources.erase(sources.begin());
			sources.push_back(oldest);
			return oldest.get();
		}

		return NULL;
	}

	void
	play_sound (const Sound& sound)
	{
		if (!sound)
			argument_error(__FILE__, __LINE__);

		SoundSource* source = next_source();
		if (!source || !*source)
			invalid_state_error(__FILE__, __LINE__);

		alSourcei(source->id, AL_BUFFER, get_sound_buffer_id(sound));
		check_error(__FILE__, __LINE__);

		alSourcePlay(source->id);
		check_error(__FILE__, __LINE__);
	}


}// Beeps
