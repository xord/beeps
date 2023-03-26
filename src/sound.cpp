#include "beeps/sound.h"


#include <limits.h>
#include "Stk.h"
#include "beeps/processor.h"
#include "beeps/exception.h"
#include "openal.h"
#include "signals.h"


#if 0
#define LOG(...) doutln(__VA_ARGS__)
#else
#define LOG(...)
#endif


namespace Beeps
{


	static ALuint get_buffer_id (const Sound& sound);


	struct SoundSource
	{

		typedef SoundSource This;

		typedef std::shared_ptr<This> Ptr;

		ALint id;

		static Ptr create ()
		{
			ALuint id_;
			alGenSources(1, &id_);
			if (!OpenAL_no_error()) return Ptr();

			return Ptr(new This(id_));
		}

		~SoundSource ()
		{
			if (!*this) return;

			ALuint id_ = id;
			alDeleteSources(1, &id_);
			OpenAL_check_error(__FILE__, __LINE__);
		}

		void play (const Sound& sound)
		{
			assert(sound);

			if (!*this)
				invalid_state_error(__FILE__, __LINE__);

			alSourcei(id, AL_BUFFER, get_buffer_id(sound));
			alSourcePlay(id);
			OpenAL_check_error(__FILE__, __LINE__);
		}

		void stop ()
		{
			if (!*this)
				invalid_state_error(__FILE__, __LINE__);

			alSourceStop(id);
			OpenAL_check_error(__FILE__, __LINE__);
		}

		bool is_playing () const
		{
			if (!*this) return false;

			ALint state = 0;
			alGetSourcei(id, AL_SOURCE_STATE, &state);
			OpenAL_check_error(__FILE__, __LINE__);

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

		private:

			SoundSource (ALint id = -1)
			:	id(id)
			{
			}

	};// SoundSource


	typedef std::vector<SoundSource::Ptr> SoundSourceList;


	namespace global
	{

		static SoundSourceList sources;

	}// global


	void
	Sound_cleanup_sources ()
	{
		global::sources.clear();
	}

	static SoundSource*
	get_next_source ()
	{
		SoundSource::Ptr source;

		auto end = global::sources.end();
		for (auto it = global::sources.begin(); it != end; ++it)
		{
			const SoundSource::Ptr& p = *it;
			if (p && *p && !p->is_playing())
			{
				source = p;
				global::sources.erase(it);
				LOG("reuse source");
				break;
			}
		}

		if (!source)
		{
			source = SoundSource::create();
			LOG("new source");
		}

		if (!source)
		{
			source = *global::sources.begin();
			if (source) source->stop();
			global::sources.erase(global::sources.begin());
			LOG("stop and reuse oldest source");
		}

		if (!source)
			return NULL;

		global::sources.push_back(source);
		return source.get();
	}


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
			OpenAL_check_error(__FILE__, __LINE__);

			id = id_;
		}

		void clear ()
		{
			if (id >= 0)
			{
				ALuint id_ = id;
				alDeleteBuffers(1, &id_);
				OpenAL_check_error(__FILE__, __LINE__);
			}

			id = -1;
		}

		bool is_valid () const
		{
			return id >= 0;
		}

	};// Sound::Data


	ALuint
	get_buffer_id (const Sound& sound)
	{
		return sound.self->id;
	}


	Sound::Sound ()
	{
	}

	Sound::Sound (
		Processor* processor, float seconds, uint nchannels, uint sampling_rate)
	{
		if (!processor || !*processor || seconds < 0 || nchannels < 1 || 2 < nchannels)
			argument_error(__FILE__, __LINE__);

		self->create();

		Signals signals = Signals_create(seconds, nchannels, sampling_rate);
		processor->process(&signals);

		stk::StkFrames* frames = Signals_get_frames(&signals);
		if (!frames)
			invalid_state_error(__FILE__, __LINE__);

		ALsizei size = frames->frames();
		if (size <= 0)
			invalid_state_error(__FILE__, __LINE__);

		std::vector<short> buffer;
		buffer.reserve(size * nchannels);
		for (ALsizei frame = 0; frame < size; ++frame)
			for (uint channel = 0; channel < nchannels; ++channel)
				buffer.push_back((*frames)(frame, channel) * SHRT_MAX);

		alBufferData(
			self->id,
			nchannels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16,
			&buffer[0],
			sizeof(short) * size,
			frames->dataRate());
		OpenAL_check_error(__FILE__, __LINE__);
	}

	Sound::~Sound ()
	{
	}

	void
	Sound::play ()
	{
		if (!*this)
			invalid_state_error(__FILE__, __LINE__);

		SoundSource* source = get_next_source();
		if (!source || !*source)
			invalid_state_error(__FILE__, __LINE__);

		source->play(*this);

#if 0
		std::string ox = "";
		for (size_t i = 0; i < global::sources.size(); ++i)
			ox += global::sources[i]->is_playing() ? 'o' : 'x';
		LOG("playing with %d sources. (%s)", global::sources.size(), ox.c_str());
#endif
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


}// Beeps
