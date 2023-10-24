#include "sound.h"


#include <limits.h>
#include <assert.h>
#include <memory>
#include <algorithm>
#include "Stk.h"
#include "beeps/beeps.h"
#include "beeps/exception.h"
#include "beeps/generator.h"
#include "openal.h"
#include "processor.h"
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

		SoundBuffer (bool create = false)
		{
			if (create) self->create();
		}

		SoundBuffer (const Signals& signals)
		{
			self->create();
			write(signals);
		}

		SoundBuffer (ALint id)
		{
			self->id = id;
		}

		void clear ()
		{
			self->clear();
		}

		uint write (const Signals& signals)
		{
			assert(signals);

			if (!*this)
				invalid_state_error(__FILE__, __LINE__);

			double sample_rate = signals.sample_rate();
			uint nchannels     = signals.nchannels();
			uint nsamples      = signals.nsamples();
			assert(sample_rate > 0 && nchannels > 0 && nsamples > 0);

			const Frames* frames = Signals_get_frames(&signals);
			assert(frames);

			std::vector<short> buffer;
			buffer.reserve(nsamples * nchannels);
			for (uint sample = 0; sample < nsamples; ++sample)
				for (uint channel = 0; channel < nchannels; ++channel)
					buffer.push_back((*frames)(sample, channel) * SHRT_MAX);

			alBufferData(
				self->id,
				nchannels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16,
				&buffer[0],
				sizeof(short) * nsamples * nchannels,
				sample_rate);
			OpenAL_check_error(__FILE__, __LINE__);

			return nsamples;
		}

		operator bool () const
		{
			return self->is_valid();
		}

		bool operator ! () const
		{
			return !operator bool();
		}

		struct Data
		{

			ALint id   = -1;

			bool owner = false;

			~Data ()
			{
				clear();
			}

			void create ()
			{
				clear();

				ALuint id_ = 0;
				alGenBuffers(1, &id_);
				OpenAL_check_error(__FILE__, __LINE__);

				id    = id_;
				owner = true;
			}

			void clear ()
			{
				if (owner && id >= 0)
				{
					ALuint id_ = id;
					alDeleteBuffers(1, &id_);
					OpenAL_check_error(__FILE__, __LINE__);
				}

				id    = -1;
				owner = false;
			}

			bool is_valid () const
			{
				return id >= 0;
			}

		};// Data

		Xot::PSharedImpl<Data> self;

	};// SoundBuffer


	struct SoundSource
	{

		void create ()
		{
			ALuint id_ = 0;
			alGenSources(1, &id_);
			if (OpenAL_no_error()) self->id = id_;
		}

		void clear ()
		{
			stop();
			self->clear();
		}

		SoundSource reuse ()
		{
			stop();
			set_gain(1);
			set_loop(false);

			SoundSource source;
			source.self.swap(self);
			return source;
		}

		void attach (const SoundBuffer& buffer)
		{
			assert(buffer);

			if (!*this)
				invalid_state_error(__FILE__, __LINE__);

			alSourcei(self->id, AL_BUFFER, buffer.self->id);
			OpenAL_check_error(__FILE__, __LINE__);
		}

		void queue (const SoundBuffer& buffer)
		{
			assert(buffer);

			ALuint id = buffer.self->id;
			alSourceQueueBuffers(self->id, 1, &id);
			OpenAL_check_error(__FILE__, __LINE__);

			LOG("queue: %d", buffer.self->id);
		}

		bool unqueue (SoundBuffer* buffer = NULL)
		{
			ALint count = 0;
			alGetSourcei(self->id, AL_BUFFERS_PROCESSED, &count);
			OpenAL_check_error(__FILE__, __LINE__);

			if (count <= 0) return false;

			ALuint id = 0;
			alSourceUnqueueBuffers(self->id, 1, &id);
			OpenAL_check_error(__FILE__, __LINE__);

			if (buffer) *buffer = SoundBuffer((ALint) id);
			return true;
		}

		void play ()
		{
			if (!*this)
				invalid_state_error(__FILE__, __LINE__);

			alSourcePlay(self->id);
			OpenAL_check_error(__FILE__, __LINE__);
		}

		void pause ()
		{
			if (!*this) return;

			alSourcePause(self->id);
			OpenAL_check_error(__FILE__, __LINE__);
		}

		void rewind ()
		{
			if (!*this) return;

			alSourceRewind(self->id);
			OpenAL_check_error(__FILE__, __LINE__);
		}

		void stop ()
		{
			if (!*this) return;

			alSourceStop(self->id);
			OpenAL_check_error(__FILE__, __LINE__);

			ALint type = 0;
			alGetSourcei(self->id, AL_SOURCE_TYPE, &type);
			OpenAL_check_error(__FILE__, __LINE__);

			if (type == AL_STREAMING)
				while (unqueue());
			else if (type == AL_STATIC)
			{
				alSourcei(self->id, AL_BUFFER, 0);
				OpenAL_check_error(__FILE__, __LINE__);
			}
		}

		SoundPlayer::State state () const
		{
			if (!*this) return SoundPlayer::STATE_UNKNOWN;

			ALint state = 0;
			alGetSourcei(self->id, AL_SOURCE_STATE, &state);
			OpenAL_check_error(__FILE__, __LINE__);

			switch (state)
			{
				case AL_PLAYING: return SoundPlayer::PLAYING;
				case AL_PAUSED:  return SoundPlayer::PAUSED;
				case AL_STOPPED: return SoundPlayer::STOPPED;
				default:         return SoundPlayer::STATE_UNKNOWN;
			}
		}

		void set_gain (float gain)
		{
			if (gain < 0)
				argument_error(__FILE__, __LINE__);

			if (!*this) return;

			alSourcef(self->id, AL_GAIN, gain);
			OpenAL_check_error(__FILE__, __LINE__);
		}

		float gain () const
		{
			float gain = 1;
			if (!*this) return gain;

			alGetSourcef(self->id, AL_GAIN, &gain);
			OpenAL_check_error(__FILE__, __LINE__);

			return gain;
		}

		void set_loop (bool loop)
		{
			if (!*this) return;

			alSourcei(self->id, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
			OpenAL_check_error(__FILE__, __LINE__);
		}

		bool loop () const
		{
			if (!*this) return false;

			ALint loop = AL_FALSE;
			alGetSourcei(self->id, AL_LOOPING, &loop);
			OpenAL_check_error(__FILE__, __LINE__);

			return loop != AL_FALSE;
		}

		operator bool () const
		{
			return self->id >= 0;
		}

		bool operator ! () const
		{
			return !operator bool();
		}

		struct Data
		{

			ALint id = -1;

			~Data ()
			{
				clear();
			}

			void clear ()
			{
				if (id < 0) return;

				ALuint id_ = id;
				alDeleteSources(1, &id_);
				OpenAL_check_error(__FILE__, __LINE__);

				id = -1;
			}

		};// Data

		Xot::PSharedImpl<Data> self;

	};// SoundSource


	struct SoundPlayer::Data
	{

		SoundSource source;

		std::vector<SoundBuffer> buffers;

		Processor::Ref processor;

		std::unique_ptr<ProcessorContext> processor_context;

		void clear ()
		{
			source.clear();

			for (auto& buffer : buffers) buffer.clear();
			buffers.clear();
		}

		void attach_signals (const Signals& signals)
		{
			assert(signals);

			SoundBuffer buffer(signals);
			source.attach(buffer);
			buffers.emplace_back(buffer);
		}

		void attach_stream (
			Processor* processor, uint nchannels, double sample_rate)
		{
			assert(processor && *processor && nchannels > 0 && sample_rate > 0);

			this->processor = processor;
			processor_context.reset(
				new ProcessorContext(sample_rate / 10, nchannels, sample_rate));

			for (int i = 0; i < 2; ++i)
			{
				SoundBuffer buffer(true);
				if (!process_stream(&buffer)) break;

				source.queue(buffer);
				buffers.emplace_back(buffer);
			}
		}

		bool process_stream (SoundBuffer* buffer)
		{
			assert(buffer && processor && processor_context);

			Signals signals = processor_context->process_signals(processor);
			if (!signals) return false;

			if (processor_context->is_finished())
				processor_context.reset();

			return buffer->write(signals) > 0;
		}

		void process_and_queue_stream_buffers ()
		{
			SoundBuffer buffer;
			while (is_streaming())
			{
				if (!source.unqueue(&buffer))
					return;

				if (!process_stream(&buffer))
					return;

				source.queue(buffer);
				if (source.state() == STOPPED) source.play();
			}
		}

		bool is_streaming () const
		{
			return processor && processor_context && *processor_context;
		}

	};// SoundPlayer::Data


	static SoundPlayer
	create_player ()
	{
		SoundPlayer player;
		player.self->source.create();
		return player;
	}

	static SoundPlayer
	reuse_player (SoundPlayer* player)
	{
		SoundPlayer newplayer;
		newplayer.self->source = player->self->source.reuse();
		player->self->clear();
		return newplayer;
	}


	namespace global
	{

		static std::vector<SoundPlayer> players;

	}// global


	static void
	remove_inactive_players ()
	{
		auto it = std::remove_if(
			global::players.begin(), global::players.end(),
			[](auto& player)
			{
				return !player || player.state() == SoundPlayer::STOPPED;
			});

		global::players.erase(it, global::players.end());
	}

	static SoundPlayer
	get_next_player ()
	{
		SoundPlayer player = create_player();
		if (player)
			LOG("new player");

		if (!player)
		{
			for (auto& p : global::players)
			{
				if (p && p.state() == SoundPlayer::STOPPED)
				{
					player = reuse_player(&p);
					LOG("reuse stopped player");
					break;
				}
			}
		}

		if (!player && !global::players.empty())
		{
			player = reuse_player(&global::players.front());
			LOG("reuse oldest player");
		}

		remove_inactive_players();

		if (player)
			global::players.emplace_back(player);

		return player;
	}

	void
	SoundPlayer_process_streams ()
	{
		for (auto& player : global::players)
		{
			if (player.self->is_streaming())
				player.self->process_and_queue_stream_buffers();
		}
	}

	void
	SoundPlayer_clear_streams ()
	{
		for (auto& player : global::players)
			player.self->clear();

		global::players.clear();
	}

	void
	stop_all_sound_players ()
	{
		for (auto& player : global::players)
			player.stop();
	}


	SoundPlayer::SoundPlayer ()
	{
	}

	SoundPlayer::~SoundPlayer ()
	{
	}

	void
	SoundPlayer::play ()
	{
		self->source.play();
	}

	void
	SoundPlayer::pause ()
	{
		self->source.pause();
	}

	void
	SoundPlayer::rewind ()
	{
		not_implemented_error(__FILE__, __LINE__);
	}

	void
	SoundPlayer::stop ()
	{
		self->source.stop();
	}

	SoundPlayer::State
	SoundPlayer::state () const
	{
		State s = self->source.state();
		if (s == STOPPED && self->is_streaming())
			return PLAYING;
		return s;
	}

	void
	SoundPlayer::set_gain (float gain)
	{
		self->source.set_gain(gain);
	}

	float
	SoundPlayer::gain () const
	{
		return self->source.gain();
	}

	void
	SoundPlayer::set_loop (bool loop)
	{
		self->source.set_loop(loop);
	}

	bool
	SoundPlayer::loop () const
	{
		return self->source.loop();
	}

	SoundPlayer::operator bool () const
	{
		return self->source;
	}

	bool
	SoundPlayer::operator ! () const
	{
		return !operator bool();
	}


	struct Sound::Data {

		float gain = 1;

		bool loop  = false;

		virtual ~Data ()
		{
		}

		virtual void attach_to (SoundPlayer* player)
		{
			not_implemented_error(__FILE__, __LINE__);
		}

		virtual double sample_rate () const
		{
			return 0;
		}

		virtual uint nchannels () const
		{
			return 0;
		}

		virtual float seconds () const
		{
			return 0;
		}

		virtual void save (const char* path) const
		{
			not_implemented_error(__FILE__, __LINE__);
		}

		virtual bool is_valid () const
		{
			return false;
		}

	};// Sound::Data


	struct SoundData : public Sound::Data
	{

		typedef Sound::Data Super;

		Signals signals;

		SoundData (
			Processor* processor, float seconds, uint nchannels, double sample_rate)
		{
			assert(
				processor && *processor &&
				seconds > 0 && nchannels > 0 && sample_rate > 0);

			ProcessorContext context(seconds * sample_rate, nchannels, sample_rate);
			Signals signals = context.process_signals(processor);
			if (!signals)
				beeps_error(__FILE__, __LINE__, "failed to process signals");

			this->signals = signals;
		}

		void attach_to (SoundPlayer* player) override
		{
			assert(player && *player);

			player->self->attach_signals(signals);
		}

		double sample_rate () const override
		{
			return signals ? signals.sample_rate() : Super::sample_rate();
		}

		uint nchannels () const override
		{
			return signals ? signals.nchannels() : Super::nchannels();
		}

		float seconds () const override
		{
			return signals ? Signals_get_seconds(signals) : Super::seconds();
		}

		void save (const char* path) const override
		{
			if (!signals)
				invalid_state_error(__FILE__, __LINE__);

			Signals_save(signals, path);
		}

		bool is_valid () const override
		{
			return signals;
		}

	};// SoundData


	struct StreamSoundData : public Sound::Data
	{

		Processor::Ref processor;

		double sample_rate_ = 0;

		uint  nchannels_ = 0;

		StreamSoundData (Processor* processor, uint nchannels, double sample_rate)
		{
			assert(processor && *processor && nchannels > 0 && sample_rate > 0);

			this->processor    = processor;
			this->sample_rate_ = sample_rate;
			this->nchannels_   = nchannels;
		}

		void attach_to (SoundPlayer* player) override
		{
			assert(player && *player);

			player->self->attach_stream(processor, nchannels_, sample_rate_);
		}

		double sample_rate () const override
		{
			return sample_rate_;
		}

		uint nchannels () const override
		{
			return nchannels_;
		}

		float seconds () const override
		{
			return -1;
		}

		bool is_valid () const override
		{
			return processor && sample_rate_ > 0 && nchannels_ > 0;
		}

	};// StreamSoundData


	Sound
	load_sound (const char* path)
	{
		FileIn* f = new FileIn(path);
		return Sound(f, f->seconds(), f->nchannels(), f->sample_rate());
	}


	Sound::Sound ()
	{
	}

	Sound::Sound (
		Processor* processor, float seconds, uint nchannels, double sample_rate)
	{
		Processor::Ref ref = processor;

		if (!processor || !*processor)
			argument_error(__FILE__, __LINE__);

		if (sample_rate <= 0) sample_rate = Beeps::sample_rate();

		if (seconds > 0)
			self.reset(new SoundData(processor, seconds, nchannels, sample_rate));
		else
			self.reset(new StreamSoundData(processor, nchannels, sample_rate));
	}

	Sound::~Sound ()
	{
	}

	SoundPlayer
	Sound::play ()
	{
		SoundPlayer player = get_next_player();
		if (!player)
			invalid_state_error(__FILE__, __LINE__);

		player.set_gain(gain());
		player.set_loop(loop());

		self->attach_to(&player);
		player.play();

#if 0
		std::string ox = "";
		for (auto& player : global::players)
			ox += player.state() == SoundPlayer::PLAYING ? 'o' : 'x';
		LOG("%d players. (%s)", global::players.size(), ox.c_str());
#endif

		return player;
	}

	double
	Sound::sample_rate () const
	{
		return self->sample_rate();
	}

	uint
	Sound::nchannels () const
	{
		return self->nchannels();
	}

	float
	Sound::seconds () const
	{
		return self->seconds();
	}

	void
	Sound::set_gain (float gain)
	{
		if (gain < 0)
			argument_error(__FILE__, __LINE__);

		self->gain = gain;
	}

	float
	Sound::gain () const
	{
		return self->gain;
	}

	void
	Sound::set_loop (bool loop)
	{
		self->loop = loop;
	}

	bool
	Sound::loop () const
	{
		return self->loop;
	}

	void
	Sound::save (const char* path) const
	{
		self->save(path);
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
