// -*- c++ -*-
#pragma once
#ifndef __BEEPS_DEFS_H__
#define __BEEPS_DEFS_H__


#include <xot/defs.h>
#include <xot/string.h>


#if defined(WIN32) && defined(GCC) && defined(BEEPS)
	#define BEEPS_EXPORT __declspec(dllexport)
#else
	#define BEEPS_EXPORT
#endif


namespace Beeps
{


	using namespace Xot::Types;

	using Xot::String;


	typedef double Sample;


}// Beeps


#endif//EOH
