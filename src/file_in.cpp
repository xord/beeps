#include "processor.h"


#include "beeps/exception.h"
#include "signals.h"


namespace Beeps
{


	struct FileIn::Data
	{

		String path;

		Signals signals;

	};// FileIn::Data


	FileIn::FileIn (const char* path)
	:	Super(true)
	{
		if (path) set_path(path);
	}

	FileIn::~FileIn ()
	{
	}

	void
	FileIn::reset ()
	{
		Super::reset();
	}

	void
	FileIn::set_path (const char* path)
	{
		if (*this)
			invalid_state_error(__FILE__, __LINE__, "path is already set");

		self->signals = Signals_load(path);
		self->path    = path;
	}

	const char*
	FileIn::path () const
	{
		return self->path;
	}

	double
	FileIn::sample_rate () const
	{
		return self->signals.sample_rate();
	}

	uint
	FileIn::nchannels () const
	{
		return self->signals.nchannels();
	}

	uint
	FileIn::nsamples () const
	{
		return self->signals.nsamples();
	}

	float
	FileIn::seconds () const
	{
		return Signals_get_seconds(self->signals);
	}

	void
	FileIn::process (Signals* signals, uint* offset)
	{
		Super::process(signals, offset);

		*offset += Signals_copy(signals, self->signals, *offset);
	}

	FileIn::operator bool () const
	{
		if (!Super::operator bool()) return false;
		return self->signals;
	}


}// Beeps
