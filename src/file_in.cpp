#include "beeps/processor.h"


#include "beeps/exception.h"
#include "signals.h"


namespace Beeps
{


	struct FileIn::Data
	{

		Signals signals;

	};// FileIn::Data


	FileIn::FileIn (const char* path)
	{
		if (path) load(path);
	}

	FileIn::~FileIn ()
	{
	}

	void
	FileIn::load (const char* path)
	{
		self->signals = Signals_load(path);
	}

	uint
	FileIn::sampling_rate () const
	{
		return self->signals.sampling_rate();
	}

	uint
	FileIn::nchannels () const
	{
		return self->signals.nchannels();
	}

	float
	FileIn::seconds () const
	{
		return self->signals.seconds();
	}

	void
	FileIn::process (Signals* signals)
	{
		Super::process(signals);

		Signals_copy(signals, self->signals);
	}

	FileIn::operator bool () const
	{
		if (!Super::operator bool()) return false;
		return self->signals;
	}


}// Beeps
