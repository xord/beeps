// -*- c++ -*-
#include "signals.h"


#include "beeps/exception.h"


namespace Beeps
{


	[[noreturn]]
	void
	objc_error (const char* file, int line, NSError* error, const char* format = NULL, ...)
	{
		if (!error)
			argument_error(__FILE__, __LINE__);

		XOT_STRINGF(format, s);
		beeps_error(
			file, line,
			Xot::stringf("%s (%s)", s.c_str(), error.localizedDescription.UTF8String));
	}


	Signals
	Signals_create (AVAudioPCMBuffer* buffer, uint capacity)
	{
		if (!buffer)
			argument_error(__FILE__, __LINE__);

		uint nchannels = buffer.format.channelCount;
		if (nchannels <= 0)
			beeps_error(__FILE__, __LINE__, "invalid nchannels %d", nchannels);

		uint nsamples = buffer.frameLength;
		if (nsamples <= 0)
			beeps_error(__FILE__, __LINE__, "invalid buffer length %d", nsamples);

		const float* const* channels = buffer.floatChannelData;
		if (!channels)
			beeps_error(__FILE__, __LINE__, "failed to get channel data");

		return Signals_create(
			channels, nsamples, nchannels, (uint) buffer.format.sampleRate, capacity);
	}

	static uint
	get_next_capacity (const Signals& signals, uint nsamples)
	{
		uint cap = signals.capacity();
		while (cap < nsamples) cap *= 2;
		return cap;
	}

	void
	Signals_append (Signals* to, AVAudioPCMBuffer* buffer)
	{
		if (!buffer)
			argument_error(__FILE__, __LINE__);

		uint nchannels = buffer.format.channelCount;
		if (nchannels <= 0)
			beeps_error(__FILE__, __LINE__, "invalid nchannels %d", nchannels);
		if (nchannels != to->nchannels())
			beeps_error(__FILE__, __LINE__, "nchannels does not match", nchannels);

		uint nsamples = buffer.frameLength;
		if (nsamples <= 0)
			beeps_error(__FILE__, __LINE__, "invalid buffer length %d", nsamples);

		const float* const* channels = buffer.floatChannelData;
		if (!channels)
			beeps_error(__FILE__, __LINE__, "failed to get channel data");

		uint new_nsamples = to->nsamples() + nsamples;
		if (new_nsamples > to->capacity())
			Signals_set_capacity(to, get_next_capacity(*to, new_nsamples));

		for (uint channel = 0; channel < nchannels; ++channel)
		{
			Sample* p = Signals_at(to, to->nsamples(), channel);
			for (uint i = 0; i < nsamples; ++i, p += nchannels)
				*p = channels[channel][i];
		}

		Signals_set_nsamples(to, new_nsamples);
	}

	static AVAudioPCMBuffer*
	load_buffer (const char* path)
	{
		if (!path)
			argument_error(__FILE__, __LINE__);

		NSURL* url = [NSURL fileURLWithPath: [NSString stringWithUTF8String:path]];
		if (!url)
			system_error(__FILE__, __LINE__, "failed to get url object for path");

		NSError* error = nil;

		AVAudioFile* file =
			[[[AVAudioFile alloc] initForReading:url error:&error] autorelease];
		if (error)
			objc_error(__FILE__, __LINE__, error, "failed to open file '%s'", path);

		AVAudioFormat* format = file.processingFormat;
		if (format.channelCount <= 0)
			beeps_error(__FILE__, __LINE__, "invalid channel count %d", format.channelCount);

		AVAudioPCMBuffer* buffer = [[[AVAudioPCMBuffer alloc]
			initWithPCMFormat:format frameCapacity:(AVAudioFrameCount) file.length]
			autorelease];

		BOOL result = [file readIntoBuffer:buffer error:&error];
		if (error)
			objc_error(__FILE__, __LINE__, error, "failed to read into the buffer");
		if (!result)
			system_error(__FILE__, __LINE__, "failed to read into the buffer");

		return buffer;
	}

	Signals
	Signals_load (const char* path)
	{
		AVAudioPCMBuffer* buffer = load_buffer(path);
		if (!buffer) return Signals();

		return Signals_create(buffer);
	}


}// Beeps
