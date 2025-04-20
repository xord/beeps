// -*- c++ -*-
#pragma once
#ifndef __BEEPS_SRC_PROCESSOR_H__
#define __BEEPS_SRC_PROCESSOR_H__


#include <map>
#include "beeps/processor.h"
#include "signals.h"


namespace Beeps
{


	class ProcessorContext;


	ProcessorContext* Processor_get_context (Processor::Context* context);

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

			ProcessorContext (uint nchannels, double sample_rate);

			void process (
				Processor* processor, Signals* signals, uint* offset,
				bool ignore_buffer = false);

		private:

			double sample_rate;

			uint nchannels;

			std::map<uintptr_t, std::unique_ptr<SignalsBuffer>> buffers;

			SignalsBuffer* get_buffer (Processor* processor);

	};// ProcessorContext


	class StreamContext
	{

		public:

			StreamContext (
				uint nsamples_per_process, uint nchannels, double sample_rate);

			Signals process_next (Processor* processor);

			//void push_offset (uint offset);

			//uint pop_offset ();

			bool is_finished () const;

		private:

			ProcessorContext context;

			Signals signals;

			uint nsamples_per_process;

			uint offset   = 0;

			bool finished = false;

			//std::vector<uint> offset_stack;

	};// StreamContext


}// Beeps


#endif//EOH
