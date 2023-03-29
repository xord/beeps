#include "beeps/processor.h"


#include <vector>
#include "signals.h"


namespace Beeps
{


	struct Pipeline::Data
	{

		std::vector<Processor::Ref> processors;

	};// Pipeline::Data


	Pipeline::Pipeline ()
	{
	}

	Pipeline::~Pipeline ()
	{
	}

	void
	Pipeline::add (Processor* processor)
	{
		if (!processor || !*processor)
			argument_error(__FILE__, __LINE__);

		self->processors.push_back(processor);
	}

	void
	Pipeline::process (Signals* signals)
	{
		Super::process(signals);

		for (auto& p : self->processors)
			p->process(signals);
	}

	Pipeline::operator bool () const
	{
		for (auto& p : self->processors)
			if (!*p) return false;
		return true;
	}


}// Beeps
