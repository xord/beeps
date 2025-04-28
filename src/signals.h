// -*- c++ -*-
#pragma once
#ifndef __BEEPS_SRC_SIGNALS_H__
#define __BEEPS_SRC_SIGNALS_H__


#include <assert.h>
#include <functional>
#include <vector>
#include <Stk.h>
#include <xot/pimpl.h>
#include "beeps/beeps.h"
#include "beeps/signals.h"
#include "beeps/exception.h"


namespace Beeps
{


	template <typename T> class SignalSamples;


	Signals Signals_create (
		uint capacity, uint nchannels = 1, double sample_rate = 0);

	Signals Signals_create (
		const float* const* channels,
		uint nsamples, uint nchannels, double sample_rate = 0);

	uint Signals_tick (
		Signals* signals,
		std::function<void(stk::StkFrames*)> fun);

	uint Signals_tick (
		Signals* signals, uint start, uint length,
		std::function<void(stk::StkFrames*)> fun);

	void Signals_clear (Signals* signals);

	void Signals_clear (Signals* signals, uint capacity);

	void Signals_fill (Signals* signals, uint nsamples, Sample value);

	uint Signals_copy (Signals* to, const Signals& from, uint from_offset);

	void Signals_add (     Signals* signals, const Signals& add);

	void Signals_multiply (Signals* signals, const Signals& multiplier);

	template <typename T>
	void Signals_write_samples (
		Signals* signals, const SignalSamples<T>& samples, long nsamples = -1);

	void  Signals_set_nsamples (Signals* signals, uint nsamples);

	      Sample* Signals_at (      Signals* signals, uint index, uint channel = 0);

	const Sample* Signals_at (const Signals* signals, uint index, uint channel = 0);

	const Sample* Signals_at (const Signals& signals, uint index, uint channel = 0);

	float Signals_get_seconds (const Signals& signals);

	void    Signals_save (const Signals& signals, const char* path);

	Signals Signals_load (const char* path);


	template <typename T = Sample>
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

				uint nsamples  = signals.nsamples();
				uint nchannels = signals.nchannels();

				auto& samples = self->samples;
				samples.reserve(nsamples * nchannels);

				for (uint channel = 0; channel < nchannels; ++channel)
				{
					const Sample* p = Signals_at(signals, 0);
					for (uint i = 0; i < nsamples; ++i, p += nchannels)
						samples.push_back(*p);
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
