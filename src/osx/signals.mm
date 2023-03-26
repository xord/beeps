// -*- c++ -*-
#include "../signals.h"


#import <AVFoundation/AVFoundation.h>
#include <beeps/exception.h>


namespace Beeps
{


	[[noreturn]]
	void
	objc_error (const char* file, int line, NSError* error, const char* format = NULL, ...)
	{
		XOT_STRINGF(format, s);
		beeps_error(
			file, line,
			Xot::stringf("%s (%s)", s.c_str(), error.localizedDescription.UTF8String));
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

		uint nchannels = buffer.format.channelCount;
		if (nchannels <= 0)
			beeps_error(__FILE__, __LINE__, "invalid nchannels %d", nchannels);

		uint len = buffer.frameLength;
		if (len <= 0)
			beeps_error(__FILE__, __LINE__, "invalid buffer length %d", len);

		const float* const* data = buffer.floatChannelData;
		if (!data)
			beeps_error(__FILE__, __LINE__, "failed to get channel data");

		return Signals_create(data, len, nchannels, (uint) buffer.format.sampleRate);
	}


}// Beeps
