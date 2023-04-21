#include "beeps/ruby/filter.h"


#include "beeps/ruby/processor.h"
#include "beeps/signals.h"
#include "defs.h"


RUCY_DEFINE_WRAPPER_VALUE_FROM_TO(Beeps::Analyser)

#define THIS  to<Beeps::Analyser*>(self)

#define CHECK RUCY_CHECK_OBJ(Beeps::Analyser, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return value(new Beeps::RubyProcessor<Beeps::Analyser>, klass);
}
RUCY_END

static
RUCY_DEF1(set_fft_size, size)
{
	CHECK;

	THIS->set_fft_size(to<uint>(size));
	return size;
}
RUCY_END

static
RUCY_DEF0(get_fft_size)
{
	CHECK;

	return value(THIS->fft_size());
}
RUCY_END

static
RUCY_DEF0(get_resolution)
{
	CHECK;

	return value(THIS->resolution());
}
RUCY_END

static
RUCY_DEF1(each_signal, nsamples_)
{
	CHECK;

	if (!*THIS)
		invalid_object_error(__FILE__, __LINE__);

	const Beeps::Signals& sig = THIS->signals();

	uint nsamples = to<uint>(nsamples_);
	if (nsamples > sig.nsamples()) nsamples = sig.nsamples();

	const double* samples = sig.samples();
	uint nchannels        = sig.nchannels();
	uint start            = (sig.nsamples() - nsamples) * nchannels;
	switch (nchannels)
	{
		case 1:
			for (uint i = start; i < nsamples; ++i)
				rb_yield(value(samples[i]));
			break;

		case 2:
			Value values[2];
			Value args(2, values);
			for (uint i = start; i < nsamples; i += 2)
			{
				args[0] = value(samples[i + 0]);
				args[1] = value(samples[i + 1]);
				rb_yield(args);
			}
			break;
	}

	return nil();
}
RUCY_END

static
RUCY_DEF0(each_spectrum)
{
	CHECK;

	if (!*THIS)
		invalid_object_error(__FILE__, __LINE__);

	for (auto val : THIS->spectrum())
		rb_yield(value(val));

	return nil();
}
RUCY_END


static Class cAnalyser;

void
Init_beeps_analyser ()
{
	Module mBeeps = define_module("Beeps");

	cAnalyser = mBeeps.define_class("Analyser", Beeps::processor_class());
	cAnalyser.define_alloc_func(alloc);
	cAnalyser.define_method("fft_size=",  set_fft_size);
	cAnalyser.define_method("fft_size",   get_fft_size);
	cAnalyser.define_method("resolution", get_resolution);
	cAnalyser.define_private_method("each_signal!",   each_signal);
	cAnalyser.define_private_method("each_spectrum!", each_spectrum);
}


namespace Beeps
{


	Class
	analyser_class ()
	{
		return cAnalyser;
	}


}// Beeps
