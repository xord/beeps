// -*- c++ -*-
#pragma once
#ifndef __BEEPS_RUBY_PROCESSOR_H__
#define __BEEPS_RUBY_PROCESSOR_H__


#include <rucy/class.h>
#include <rucy/extension.h>
#include <beeps/processor.h>


RUCY_DECLARE_WRAPPER_VALUE_FROM_TO(BEEPS_EXPORT, Beeps::Processor)


namespace Beeps
{


	BEEPS_EXPORT Rucy::Class processor_class ();
	// class Beeps::Processor


	template <typename T>
	class RubyProcessor : public Rucy::ClassWrapper<T> {};


}// Beeps


namespace Rucy
{


	template <> inline Class
	get_ruby_class<Beeps::Processor> ()
	{
		return Beeps::processor_class();
	}


}// Rucy


#endif//EOH
