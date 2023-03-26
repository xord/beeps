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
		float seconds, uint nchannels = 1, uint sampling_rate = 0);

	Signals Signals_create (
		const float* const* channels,
		uint nsamples, uint nchannels, uint sampling_rate = 0);

	Signals Signals_load (const char* path);

	template <typename T>
	void Signals_set_buffer (Signals* signals, const SignalBuffer<T>& buffer);

	bool Signals_copy (
		Signals* to, const Signals& from, float from_offset_sec = 0);


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

				self->nsamples = signals.nsamples();

				uint nframes   = f->frames();
				uint nchannels = f->channels();

				auto& buffer = self->buffer;
				buffer.reserve(nframes * nchannels);

				for (uint channel = 0; channel < nchannels; ++channel)
					for (uint frame = 0; frame < nframes; ++frame)
						buffer.push_back((*f)(frame, channel));

				for (uint i = 0; i < nchannels; ++i)
					self->channels.push_back(&self->buffer[nframes * i]);
			}

			uint nsamples () const
			{
				return self->nsamples;
			}

			uint nchannels () const
			{
				return (uint) self->channels.size();
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
