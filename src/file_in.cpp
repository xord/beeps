#include "beeps/processor.h"


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
	:	Super(false)
	{
		if (path) set_path(path);
	}

	FileIn::~FileIn ()
	{
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

	uint
	FileIn::sample_rate () const
	{
		return self->signals.sample_rate();
	}

	uint
	FileIn::nsamples () const
	{
		return self->signals.nsamples();
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
