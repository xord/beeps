#include "mic_in.h"


#include <limits.h>
#include <vector>
#include <memory>
#include <algorithm>
#include "beeps/beeps.h"
#include "beeps/exception.h"
#include "openal.h"
#include "signals.h"


namespace Beeps
{


	struct Mic
	{

		void start ()
		{
			if (self->started) return;

			self->create();
			if (!is_valid())
				invalid_state_error(__FILE__, __LINE__);

			alcCaptureStart(self->device);
			OpenAL_check_error(__FILE__, __LINE__);

			self->started = true;
		}

		void stop ()
		{
			if (!self->started) return;

			if (!is_valid())
				invalid_state_error(__FILE__, __LINE__);

			alcCaptureStop(self->device);
			OpenAL_check_error(__FILE__, __LINE__);

			self->started = false;
		}

		bool is_started () const
		{
			return self->started;
		}

		void buffer_samples ()
		{
			if (!is_valid())
				invalid_state_error(__FILE__, __LINE__);

			ALint nsamples = 0;
			alcGetIntegerv(
				self->device, ALC_CAPTURE_SAMPLES, sizeof(nsamples), &nsamples);
			OpenAL_check_error(__FILE__, __LINE__);
			if (nsamples <= 0) return;

			auto& buffer = self->buffer;
			if (buffer.size() < (size_t) nsamples) buffer.resize(nsamples);

			alcCaptureSamples(self->device, (ALCvoid*) &buffer[0], nsamples);
			OpenAL_check_error(__FILE__, __LINE__);

			auto& samples = self->samples;
			samples.reserve(samples.size() + nsamples);
			samples.insert(samples.end(), &buffer[0], &buffer[0] + nsamples);

			if (samples.size() > samples_size_max())
			{
				size_t size = samples.size() - samples_size_max();
				samples.erase(samples.begin(), samples.begin() + size);
				self->offset += (uint) size;
			}
		}

		void get_signals (Signals* signals, uint* gen_offset)
		{
			assert(signals && signals->empty() && generate_offset);

			if (!is_valid()) return;

			Frames* frames = Signals_get_frames(signals);
			assert(frames);

			auto& samples  = self->samples;
			uint nchannels = self->nchannels;
			uint nsamples  = (uint) (samples.size() / nchannels);
			uint offset    = *gen_offset < self->offset ? 0 : *gen_offset - self->offset;
			if (offset >= nsamples) return;

			nsamples = std::min(frames->nframes(), nsamples - offset);
			for (uint ch = 0; ch < frames->nchannels(); ++ch)
			{
				uint samples_ch = ch < nchannels ? ch : 0;
				float div       = 1.0f / SHRT_MAX;
				for (uint i = 0; i < nsamples; ++i)
					(*frames)(i, ch) = samples[(offset + i) * nchannels + samples_ch] * div;
			}
			Signals_set_nsamples(signals, nsamples);

			*gen_offset = self->offset + offset + nsamples;
		}

		uint samples_size_max () const
		{
			assert(self->sample_rate > 0);

			return self->sample_rate * 1;
		}

		bool is_valid () const
		{
			return self->sample_rate > 0 && self->nchannels > 0 && self->device;
		}

		struct Data
		{

			double sample_rate = 0;

			uint nchannels     = 0;

			uint offset        = 0;

			bool started       = false;

			ALCdevice* device  = NULL;

			std::vector<ALshort> samples, buffer;

			~Data ()
			{
				clear();
			}

			void create ()
			{
				assert(sample_rate > 0 && nchannels > 0);

				if (device) return;

				device = alcCaptureOpenDevice(
					NULL,
					sample_rate,
					nchannels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16,
					sample_rate / 10);
				if (!device)
					openal_error(__FILE__, __LINE__, "failed to create a mic device");
			}

			void clear ()
			{
				if (!device) return;

				if (alcCaptureCloseDevice(device) == ALC_FALSE)
					openal_error(__FILE__, __LINE__, "failed to close the mic device");

				device = NULL;
			}

		};// Data

		Xot::PSharedImpl<Data> self;

	};// Mic


	struct MicIn::Data
	{

		Mic mic;

	};// MicIn::Data


	namespace global
	{

		static std::vector<Mic> mics;

	};// global


	void
	MicIn_process_streams ()
	{
		for (Mic& mic : global::mics)
			mic.buffer_samples();
	}


	MicIn::MicIn (uint nchannels, double sample_rate)
	{
		if (nchannels > 2)
			argument_error(__FILE__, __LINE__);

		if (sample_rate <= 0) sample_rate = Beeps::sample_rate();

		self->mic.self->sample_rate = sample_rate;
		self->mic.self->nchannels   = nchannels;
	}

	MicIn::~MicIn ()
	{
	}

	void
	MicIn::start ()
	{
		if (self->mic.is_started()) return;

		self->mic.start();

		global::mics.emplace_back(self->mic);
	}

	void
	MicIn::stop ()
	{
		if (!self->mic.is_started()) return;

		std::erase_if(
			global::mics,
			[&](auto& mic) {return mic.self.get() == self->mic.self.get();});

		self->mic.stop();
	}

	double
	MicIn::sample_rate () const
	{
		return self->mic.self->sample_rate;
	}

	uint
	MicIn::nchannels () const
	{
		return self->mic.self->nchannels;
	}

	void
	MicIn::generate (Context* context, Signals* signals, uint* offset)
	{
		Super::generate(context, signals, offset);

		self->mic.get_signals(signals, offset);
	}

	MicIn::operator bool () const
	{
		if (!Super::operator bool()) return false;
		return self->mic.is_valid();
	}


}// Beeps
