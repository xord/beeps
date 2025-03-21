// -*- c++ -*-
#pragma once
#ifndef __BEEPS_SRC_SIGNALS_H__
#define __BEEPS_SRC_SIGNALS_H__


#include <assert.h>
#include <vector>
#include <Stk.h>
#include <xot/pimpl.h>
#include "beeps/signals.h"
#include "beeps/exception.h"


namespace Beeps
{


	typedef stk::StkFloat Float;


	class Frames;

	template <typename T> class SignalSamples;


	Signals Signals_create (
		uint capacity, uint nchannels = 1, double sample_rate = 0);

	Signals Signals_create (
		const float* const* channels,
		uint nsamples, uint nchannels, double sample_rate = 0);

	void Signals_clear (Signals* signals);

	void Signals_clear (Signals* signals, uint capacity);

	void Signals_resize (Signals* signals, uint nsamples, Float value);

	uint Signals_copy (Signals* to, const Signals& from, uint from_offset);

	void Signals_add (     Signals* signals, const Signals& add);

	void Signals_multiply (Signals* signals, const Signals& multiplier);

	template <typename T>
	void Signals_write_samples (
		Signals* signals, const SignalSamples<T>& samples, long nsamples = -1);

	void  Signals_set_nsamples (Signals* signals, uint nsamples);

	float Signals_get_seconds (const Signals& signals);

	      Frames* Signals_get_frames (      Signals* signals);

	const Frames* Signals_get_frames (const Signals* signals);

	void    Signals_save (const Signals& signals, const char* path);

	Signals Signals_load (const char* path);


	class Frames : public stk::StkFrames
	{

		public:

			Frames (unsigned int nframes = 0, unsigned int nchannels = 1)
			:	stk::StkFrames(nframes, nchannels)
			{
			}

			size_t slice (size_t start, size_t length)
			{
				assert(!saved_data_);

				size_t end = start + length;
				assert(start <= nFrames_ && end <= nFrames_);

				saved_data_       = data_;
				saved_nFrames_    = nFrames_;
				saved_size_       = size_;
				saved_bufferSize_ = bufferSize_;

				data_      += start * nChannels_;
				nFrames_    = length;
				size_       = length * nChannels_;
				bufferSize_ = size_;

				return end;
			}

			void unslice ()
			{
				assert(saved_data_);

				data_       = saved_data_;
				nFrames_    = saved_nFrames_;
				size_       = saved_size_;
				bufferSize_ = saved_bufferSize_;

				saved_data_       = NULL;
				saved_nFrames_    = 0;
				saved_size_       = 0;
				saved_bufferSize_ = 0;
			}

			uint sample_rate () const
			{
				return dataRate();
			}

			uint nchannels () const
			{
				return nChannels_;
			}

			uint nframes () const
			{
				return (uint) nFrames_;
			}

			const Float* data () const
			{
				return data_;
			}

		private:

			Float* saved_data_ = NULL;

			size_t saved_nFrames_ = 0, saved_size_ = 0, saved_bufferSize_ = 0;

	};// Frames


	template <typename T>
	class SignalSamples
	{

		typedef SignalSamples<T> This;

		public:

			SignalSamples (uint nsamples, uint nchannels)
			{
				self->nsamples = nsamples;
				self->samples.resize(nsamples * nchannels, 0);
				self->update_channels(nchannels);
			}

			SignalSamples (const Signals& signals)
			{
				if (!signals)
					argument_error(__FILE__, __LINE__);

				const Frames* f = Signals_get_frames(&signals);
				assert(f);

				uint nsamples  = signals.nsamples();
				uint nchannels = signals.nchannels();

				auto& samples = self->samples;
				samples.reserve(nsamples * nchannels);

				for (uint channel = 0; channel < nchannels; ++channel)
				{
					for (uint sample = 0; sample < nsamples; ++sample)
						samples.push_back((*f)(sample, channel));
				}

				self->nsamples = nsamples;
				self->update_channels(nchannels);
			}

			uint nchannels () const
			{
				return (uint) self->channels.size();
			}

			uint nsamples () const
			{
				return self->nsamples;
			}

			      T* const* channels () {return &self->channels[0];}

			const T* const* channels () const
			{
				return const_cast<This*>(this)->channels();
			}

			      T* channel (uint index) {return self->channels[index];}

			const T* channel (uint index) const
			{
				return const_cast<This*>(this)->channel(index);
			}

			struct Data
			{

				uint nsamples = 0;

				std::vector<T> samples;

				std::vector<T*> channels;

				void update_channels (uint nchannels)
				{
					channels.clear();
					for (uint i = 0; i < nchannels; ++i)
						channels.push_back(&samples[nsamples * i]);
				}

			};// Data

			Xot::PSharedImpl<Data> self;

	};// SignalSamples


}// Beeps


#endif//EOH
