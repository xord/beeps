#include "beeps/processor.h"


#include "SineWave.h"
#include "FileWvIn.h"
#include "beeps/signals.h"
#include "beeps/exception.h"


namespace Beeps
{


	Processor::Processor ()
	{
	}

	Processor::~Processor ()
	{
	}

	void
	Processor::process (Signals* signals)
	{
		if (!signals || !*signals)
			argument_error(__FILE__, __LINE__);

		if (!*this)
			invalid_state_error(__FILE__, __LINE__);
	}

	Processor::operator bool () const
	{
		return true;
	}

	bool
	Processor::operator ! () const
	{
		return !operator bool();
	}


	struct SineWave::Data
	{

		stk::SineWave oscillator;

	};// SineWave::Data


	SineWave::SineWave ()
	{
	}

	SineWave::~SineWave ()
	{
	}

	void
	SineWave::process (Signals* signals)
	{
		Super::process(signals);

		self->oscillator.tick(*signals->frames());
	}


	struct FileInput::Data
	{

		stk::FileWvIn input;

	};// FileInput::Data


	FileInput::FileInput (const char* path)
	{
		if (path)
			self->input.openFile(path);
	}

	FileInput::~FileInput ()
	{
	}

	void
	FileInput::process (Signals* signals)
	{
		Super::process(signals);

		self->input.tick(*signals->frames());
	}

	FileInput::operator bool () const
	{
		return self->input.isOpen();
	}


}// Beeps
