// -*- c++ -*-
#pragma once
#ifndef __BEEPS_SRC_PROCESSOR_H__
#define __BEEPS_SRC_PROCESSOR_H__


#include <map>
#include <beeps/processor.h>
#include "signals.h"


namespace Beeps
{


	class ProcessorContext;


	float Processor_get_buffering_seconds (Processor* processor);


	class SignalsBuffer
	{

		public:

			SignalsBuffer (
				uint nsamples_per_block, uint nchannels, double sample_rate);

			void process (
				ProcessorContext* context,
				Processor* processor, Signals* signals, uint* offset);

			operator bool () const;

			bool operator ! () const;

		private:

			Signals buffer;

			uint buffer_offset = 0;

			double last_update_time = 0;

			void buffer_next (
				ProcessorContext* context, Processor* processor, uint offset);

			void clear ();

	};// SignalsBuffer


	class ProcessorContext : public Processor::Context
	{

		public:

			ProcessorContext (
				uint nsamples_per_process, uint nchannels, double sample_rate);

			Signals process_signals (Processor* processor);

			void process (
				Processor* processor, Signals* signals, uint* offset,
				bool ignore_buffer = false);

			//void push_offset (uint offset);

			//uint pop_offset ();

			bool is_finished () const;

			operator bool () const;

			bool operator ! () const;

		private:

			Signals signals;

			uint offset = 0;

			bool finished = false;

			//std::vector<uint> offset_stack;

			std::map<uintptr_t, std::unique_ptr<SignalsBuffer>> buffers;

			SignalsBuffer* get_buffer (Processor* processor);

	};// ProcessorContext


}// Beeps


#endif//EOH
