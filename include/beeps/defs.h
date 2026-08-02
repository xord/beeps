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


	namespace Types
	{


		typedef double Sample;


	}// Types


	using namespace Xot  ::Types;

	using namespace Beeps::Types;


	using Xot::String;

	using Xot::StringList;


}// Beeps


#endif//EOH
