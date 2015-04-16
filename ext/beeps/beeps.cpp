#include <rucy.h>
#include "beeps/beeps.h"
#include "defs.h"


using namespace Rucy;


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
	Beeps::fin();
	return self;
}
RUCY_END


static Module mBeeps;

void
Init_beeps ()
{
	mBeeps = define_module("Beeps");
	mBeeps.define_singleton_method("init!", init);
	mBeeps.define_singleton_method("fin!", fin);
}


namespace Beeps
{


	Module
	beeps_module ()
	{
		return mBeeps;
	}


}// Beeps
