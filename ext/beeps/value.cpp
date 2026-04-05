#include "beeps/ruby/generator.h"


#include "beeps/ruby/processor.h"
#include "defs.h"


RUCY_DEFINE_WRAPPER_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::Value)

#define THIS  to<Beeps::Value*>(self)

#define CHECK RUCY_CHECK_OBJ(Beeps::Value, self)


static
RUCY_DEF_ALLOC(alloc, klass)
{
	return value(new Beeps::RubyProcessor<Beeps::Value>, klass);
}
RUCY_END

static
RUCY_DEF1(set_type, type)
{
	CHECK;

	THIS->set_type((Beeps::Value::Type) to<uint>(type));
	return type;
}
RUCY_END

static
RUCY_DEF0(get_type)
{
	CHECK;

	return value(THIS->type());
}
RUCY_END

static
RUCY_DEF1(set_value, value)
{
	CHECK;

	THIS->set_value(to<float>(value));
	return value;
}
RUCY_END

static
RUCY_DEF2(insert, value, time)
{
	CHECK;

	THIS->insert(to<float>(value), to<float>(time));
}
RUCY_END

static
RUCY_DEF0(clear)
{
	CHECK;

	THIS->clear();
}
RUCY_END

static
RUCY_DEF0(each_value)
{
	CHECK;

	Value ret;
	for (auto it = THIS->begin(), end = THIS->end(); it != end; ++it)
		ret = rb_yield_values(2, value(it->value), value(it->time));
	return ret;
}
RUCY_END


static Class cValue;

void
Init_beeps_value ()
{
	Module mBeeps = define_module("Beeps");

	cValue = mBeeps.define_class("Value", Beeps::processor_class());
	cValue.define_alloc_func(alloc);
	cValue.define_method("type=",  set_type);
	cValue.define_method("type",   get_type);
	cValue.define_method("value=", set_value);
	cValue.define_method("insert", insert);
	cValue.define_method("clear",  clear);
	cValue.define_method("each_value!", each_value);

	cValue.define_const("TYPE_NONE", Beeps::Value::TYPE_NONE);
	cValue.define_const("LINEAR",    Beeps::Value::LINEAR);
}


namespace Beeps
{


	Class
	value_class ()
	{
		return cValue;
	}


}// Beeps
