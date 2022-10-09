// -*- c++ -*-
#pragma once
#ifndef __BEEPS_RUBY_DEFS_H__
#define __BEEPS_RUBY_DEFS_H__


#include <rucy/extension.h>
#include <beeps/ruby/exception.h>


#define BEEPS_CATCH \
	} \
	catch (const Beeps::OpenALError& e) \
	{ \
		RUCY_RAISE(Beeps::openal_error_class(), e.what()); \
	} \
	catch (const Beeps::BeepsError& e) \
	{ \
		RUCY_RAISE(Beeps::beeps_error_class(), e.what());

#define RUCY_END \
	BEEPS_CATCH \
	RUCY_DEF_END


#endif//EOH
