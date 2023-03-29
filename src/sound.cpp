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


	struct SoundBuffer
	{

		ALint id;

		SoundBuffer ()
		:	id(-1)
		{
		}

		~SoundBuffer ()
		{
			clear();
		}

		void write (const Signals& signals)
		{
			if (!signals)
				argument_error(__FILE__, __LINE__);

			uint sampling_rate = signals.sampling_rate();
			uint nsamples      = signals.nsamples();
			uint nchannels     = signals.nchannels();
			if (sampling_rate <= 0 || nsamples <= 0 || nchannels <= 0)
				argument_error(__FILE__, __LINE__);

			const stk::StkFrames* frames = Signals_get_frames(&signals);
			if (!frames)
				argument_error(__FILE__, __LINE__);

			std::vector<short> buffer;
			buffer.reserve(nsamples * nchannels);
			for (uint sample = 0; sample < nsamples; ++sample)
				for (uint channel = 0; channel < nchannels; ++channel)
					buffer.push_back((*frames)(sample, channel) * SHRT_MAX);

			clear();
			create();

			alBufferData(
				id,
				nchannels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16,
				&buffer[0],
				sizeof(short) * nsamples,
				sampling_rate);
			OpenAL_check_error(__FILE__, __LINE__);
		}

		bool is_valid () const
		{
			return id >= 0;
		}

		private:

			void create ()
			{
				if (is_valid())
					invalid_state_error(__FILE__, __LINE__);

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

	};// SoundBuffer


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

		void play (const SoundBuffer& buffer)
		{
			assert(buffer.is_valid());

			if (!*this)
				invalid_state_error(__FILE__, __LINE__);

			alSourcei(id, AL_BUFFER, buffer.id);
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

		uint nchannels = 0, sampling_rate = 0;

		Processor::Ref processor;

		SoundBuffer buffer;

		void process (float seconds)
		{
			if (seconds < 0)
				argument_error(__FILE__, __LINE__);

			if (!is_valid())
				invalid_state_error(__FILE__, __LINE__);

			Signals signals = Signals_create(seconds, nchannels, sampling_rate);
			processor->process(&signals);

			buffer.write(signals);
		}

		bool is_valid () const
		{
			return
				(nchannels == 1 || nchannels == 2) &&
				processor && *processor;
		}

	};// Sound::Data


	Sound::Sound ()
	{
	}

	Sound::Sound (
		Processor* processor, float seconds, uint nchannels, uint sampling_rate)
	{
		if (!processor || !*processor || nchannels <= 0)
			argument_error(__FILE__, __LINE__);

		self->processor     = processor;
		self->nchannels     = nchannels;
		self->sampling_rate = sampling_rate;

		if (seconds > 0) self->process(seconds);
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

		source->play(self->buffer);

#if 0
		std::string ox = "";
		for (size_t i = 0; i < global::sources.size(); ++i)
			ox += global::sources[i]->is_playing() ? 'o' : 'x';
		LOG("playing with %d sources. (%s)", global::sources.size(), ox.c_str());
#endif
	}

	Sound::operator bool () const
	{
		return self->is_valid() && self->buffer.is_valid();
	}

	bool
	Sound::operator ! () const
	{
		return !operator bool();
	}


}// Beeps
