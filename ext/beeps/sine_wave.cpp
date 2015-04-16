#include "beeps/ruby/processor.h"


#include <rucy.h>
#include "beeps/exception.h"
#include "defs.h"


using namespace Rucy;


RUCY_DEFINE_VALUE_FROM_TO(Beeps::SineWave)

#define THIS  to<Beeps::SineWave*>(self)

#define CHECK RUCY_CHECK_OBJECT(Beeps::SineWave, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return new_type<Beeps::SineWave>(klass);
}
RUCY_END


static Class cSineWave;

void
Init_sine_wave ()
{
	Module mBeeps = define_module("Beeps");

	cSineWave = mBeeps.define_class("SineWave", Beeps::processor_class());
	cSineWave.define_alloc_func(alloc);
}


namespace Beeps
{


	Class
	sine_wave_class ()
	{
		return cSineWave;
	}


}// Beeps
