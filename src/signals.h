// -*- c++ -*-
#pragma once
#ifndef __BEEPS_SRC_SIGNALS_H__
#define __BEEPS_SRC_SIGNALS_H__


#include <vector>
#include <Stk.h>
#include <xot/pimpl.h>
#include <beeps/signals.h>
#include <beeps/exception.h>


namespace Beeps
{


	template <typename T> class SignalBuffer;


	      stk::StkFrames* Signals_get_frames (      Signals* signals);

	const stk::StkFrames* Signals_get_frames (const Signals* signals);

	Signals Signals_create (
		uint capacity, uint nchannels = 1, double sample_rate = 0);

	Signals Signals_create (
		const float* const* channels,
		uint nsamples, uint nchannels, double sample_rate = 0);

	uint Signals_copy (Signals* to, const Signals& from, uint from_offset);

	template <typename T>
	void Signals_write_buffer (Signals* signals, const SignalBuffer<T>& buffer);

	void  Signals_set_nsamples (Signals* signals, uint nsamples);

	float Signals_get_seconds (const Signals& signals);

	void    Signals_save (const Signals& signals, const char* path);

	Signals Signals_load (const char* path);


	template <typename T>
	class SignalBuffer
	{

		typedef SignalBuffer<T> This;

		public:

			SignalBuffer (uint nsamples, uint nchannels)
			{
				self->nsamples = nsamples;
				self->buffer.resize(nsamples * nchannels, 0);
				for (uint i = 0; i < nchannels; ++i)
					self->channels.push_back(&self->buffer[nsamples * i]);
			}

			SignalBuffer (const Signals& signals)
			{
				const stk::StkFrames* f = Signals_get_frames(&signals);
				if (!f)
					invalid_state_error(__FILE__, __LINE__);

				uint nsamples  = signals.nsamples();
				uint nchannels = signals.nchannels();

				auto& buffer = self->buffer;
				buffer.reserve(nsamples * nchannels);

				for (uint channel = 0; channel < nchannels; ++channel)
					for (uint sample = 0; sample < nsamples; ++sample)
						buffer.push_back((*f)(sample, channel));

				self->nsamples = nsamples;
				for (uint i = 0; i < nchannels; ++i)
					self->channels.push_back(&self->buffer[nsamples * i]);
			}

			uint nchannels () const
			{
				return (uint) self->channels.size();
			}

			uint nsamples () const
			{
				return self->nsamples;
			}

			T* const* channels ()
			{
				return &self->channels[0];
			}

			const T* const* channels () const
			{
				return const_cast<This*>(this)->channels();
			}

			T* channel (uint index)
			{
				return self->channels[index];
			}

			const T* channel (uint index) const
			{
				return const_cast<This*>(this)->channel(index);
			}

			struct Data
			{

				uint nsamples = 0;

				std::vector<T> buffer;

				std::vector<T*> channels;

			};// Data

			Xot::PSharedImpl<Data> self;

	};// SignalBuffer


}// Beeps


#endif//EOH
