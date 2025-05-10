#include "beeps/generator.h"


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

		set_updated();
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

	FileIn::operator bool () const
	{
		if (!Super::operator bool()) return false;
		return self->signals;
	}

	void
	FileIn::generate (Context* context, Signals* signals, uint* offset)
	{
		Super::generate(context, signals, offset);

		*offset += Signals_copy(signals, self->signals, *offset);
	}


}// Beeps
