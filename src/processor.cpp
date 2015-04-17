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

		float frequency;

	};// SineWave::Data


	SineWave::SineWave ()
	{
		set_frequency(440);
	}

	SineWave::~SineWave ()
	{
	}

	void
	SineWave::set_frequency (float frequency)
	{
		self->frequency = frequency;
		self->oscillator.setFrequency(frequency);
	}

	float
	SineWave::frequency () const
	{
		return self->frequency;
	}

	void
	SineWave::process (Signals* signals)
	{
		Super::process(signals);

		self->oscillator.tick(*signals->frames());
	}


	struct FileIn::Data
	{

		stk::FileWvIn input;

	};// FileIn::Data


	FileIn::FileIn (const char* path)
	{
		if (path)
			self->input.openFile(path);
	}

	FileIn::~FileIn ()
	{
	}

	void
	FileIn::process (Signals* signals)
	{
		Super::process(signals);

		self->input.tick(*signals->frames());
	}

	FileIn::operator bool () const
	{
		return self->input.isOpen();
	}


}// Beeps
