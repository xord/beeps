#include "beeps/ruby/signals.h"


#include "defs.h"


RUCY_DEFINE_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::Signals)

#define THIS  to<Beeps::Signals*>(self)

#define CHECK RUCY_CHECK_OBJ(Beeps::Signals, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return new_type<Beeps::Signals>(klass);
}
RUCY_END

static
RUCY_DEF1(initialize_copy, obj)
{
	CHECK;
	*THIS = to<Beeps::Signals*>(obj)->dup();
	return self;
}
RUCY_END

static
RUCY_DEF0(get_sample_rate)
{
	CHECK;

	return value(THIS->sample_rate());
}
RUCY_END

static
RUCY_DEF0(get_nchannels)
{
	CHECK;

	return value(THIS->nchannels());
}
RUCY_END

static
RUCY_DEF0(get_nsamples)
{
	CHECK;

	return value(THIS->nsamples());
}
RUCY_END

static
RUCY_DEF0(get_capacity)
{
	CHECK;

	return value(THIS->capacity());
}
RUCY_END

static
RUCY_DEF0(is_empty)
{
	CHECK;

	return value(THIS->empty());
}
RUCY_END

static
RUCY_DEF1(each, channel_)
{
	CHECK;

	uint       nsamples = THIS->nsamples();
	const auto* samples = THIS->samples();

	if (channel_)
	{
		uint nchannels = THIS->nchannels();
		int   channel  = to<int>(channel_);
		if (channel < 0)
			argument_error(__FILE__, __LINE__);
		if (channel >= nchannels)
			argument_error(__FILE__, __LINE__);

		for (uint i = channel; i < nsamples; i += nchannels)
			rb_yield(value(samples[i]));
	}
	else
	{
		for (uint i = 0; i < nsamples; ++i)
			rb_yield(value(samples[i]));
	}
}
RUCY_END


static Class cSignals;

void
Init_beeps_signals ()
{
	Module mBeeps = define_module("Beeps");

	cSignals = mBeeps.define_class("Signals");
	cSignals.define_alloc_func(alloc);
	cSignals.define_private_method("initialize_copy", initialize_copy);
	cSignals.define_method("sample_rate", get_sample_rate);
	cSignals.define_method("nchannels",   get_nchannels);
	cSignals.define_method("nsamples",    get_nsamples);
	cSignals.define_method("capacity",    get_capacity);
	cSignals.define_method("empty?", is_empty);
	cSignals.define_method("each!", each);
}


namespace Beeps
{


	Class
	signals_class ()
	{
		return cSignals;
	}


}// Beeps
