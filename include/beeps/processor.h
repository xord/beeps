// -*- c++ -*-
#pragma once
#ifndef __BEEPS_PROCESSOR_H__
#define __BEEPS_PROCESSOR_H__


#include <xot/ref.h>
#include <xot/pimpl.h>
#include <beeps/defs.h>
#include <beeps/signals.h>


namespace Beeps
{


	class Processor : public Xot::RefCountable<>
	{

		typedef Processor This;

		public:

			class Context {};

			typedef Xot::Ref<This>   Ref;

			typedef std::vector<Ref> List;

			virtual ~Processor ();

			virtual void reset ();

			virtual void         set_input (Processor* input);

			virtual       Processor* input ();

			virtual const Processor* input () const;

			virtual void on_start ();

			virtual operator bool () const;

			virtual bool operator ! () const;

			struct Data;

			Xot::PImpl<Data> self;

		protected:

			Processor (bool generator = false);

			virtual void process  (Context* context, Signals* signals, uint* offset) final;

			virtual void generate (Context* context, Signals* signals, uint* offset);

			virtual void filter   (Context* context, Signals* signals, uint* offset);

			virtual void set_updated ();

			virtual void    add_sub_input (Processor* input);

			virtual void remove_sub_input (Processor* input);

			virtual const List& sub_inputs () const;

			friend class ProcessorContext;

	};// Processor


	class Generator : public Processor
	{

		typedef Processor Super;

		protected:

			Generator ();

		private:

			void filter (
				Context* context, Signals* signals, uint* offset) override final;

	};// Generator


	class Filter : public Processor
	{

		typedef Processor Super;

		protected:

			Filter (Processor* input = NULL);

			void set_buffering_seconds (float seconds);

		private:

			void generate (
				Context* context, Signals* signals, uint* offset) override final;

	};// Filter


	Signals get_signals (
		Processor* processor, float seconds,
		uint nchannels = 1, double sample_rate = 0);


}// Beeps


#endif//EOH
