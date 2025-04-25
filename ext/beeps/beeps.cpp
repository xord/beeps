#include "beeps/ruby/beeps.h"


#include "defs.h"


static
RUCY_DEF0(init)
{
	Beeps::init();
	return self;
}
RUCY_END

static
RUCY_DEF0(fin)
{
	rb_gc_start();
	Beeps::fin();
	return self;
}
RUCY_END

static
RUCY_DEF0(get_sample_rate)
{
	return value(Beeps::sample_rate());
}
RUCY_END

static
RUCY_DEF0(process_streams)
{
	Beeps::process_streams();
	return self;
}
RUCY_END


static Module mBeeps;

void
Init_beeps ()
{
	mBeeps = define_module("Beeps");
	mBeeps.define_singleton_method("init!", init);
	mBeeps.define_singleton_method("fin!",  fin);
	mBeeps.define_singleton_method("sample_rate", get_sample_rate);
	mBeeps.define_singleton_method("process_streams!", process_streams);
}


namespace Beeps
{


	Module
	beeps_module ()
	{
		return mBeeps;
	}


}// Beeps
