// -*- c++ -*-
#include "beeps/generator.h"


#include <deque>
#import <Foundation/Foundation.h>
#include "beeps/exception.h"
#include "beeps/debug.h"
#include "signals.h"


namespace Beeps
{


	typedef Xot::Ref<TextIn> TextInRef;


	struct TextIn::Data
	{

		double sample_rate = 0;

		bool finished      = false;

		Signals signals;

	};// TextIn::Data


	TextIn::TextIn (const char* text, double sample_rate)
	{
		if (sample_rate == 0)
			sample_rate = Beeps::sample_rate();
		if (sample_rate <= 0)
			argument_error(__FILE__, __LINE__);

		self->sample_rate = sample_rate;

		if (text) synthesize(text);
	}

	TextIn::~TextIn ()
	{
	}

	void
	TextIn::synthesize (const char* text)
	{
		if (@available(macOS 10.15, *))
		{
			TextInRef pthis            = this;
			AVSpeechSynthesizer* synth = [[AVSpeechSynthesizer alloc] init];

			NSString* str = [NSString stringWithUTF8String: text];
			auto* utt     = [[[AVSpeechUtterance alloc] initWithString: str] autorelease];
			utt.voice     = [AVSpeechSynthesisVoice voiceWithLanguage: @"ja-JP"];
			utt.rate      = 0.5;

			[synth writeUtterance: utt
				toBufferCallback: ^(AVAudioBuffer* buffer)
				{
					auto finish = [&]()
					{
						pthis->self->finished = true;
						[synth release];
					};

					if (![buffer isKindOfClass: AVAudioPCMBuffer.class])
						return finish();

					AVAudioPCMBuffer* pcmbuf = (AVAudioPCMBuffer*) buffer;
					if (pcmbuf.frameLength == 0)
						return finish();

					[pcmbuf retain];
					dispatch_async(dispatch_get_main_queue(), ^()
					{
						if (!pthis->self->signals)
							pthis->self->signals = Signals_create(pcmbuf, 4096);
						else
							Signals_append(&pthis->self->signals, pcmbuf);
						[pcmbuf release];
					});
				}];
		}
		else
			not_implemented_error(__FILE__, __LINE__);
	}

	double
	TextIn::sample_rate () const
	{
		return self->sample_rate;
	}

	TextIn::operator bool () const
	{
		if (!Super::operator bool()) return false;
		return self->sample_rate > 0;
	}

	void
	TextIn::generate (Context* context, Signals* signals, uint* offset)
	{
		Super::generate(context, signals, offset);

		if (self->signals)
		{
			if (signals->sample_rate() != self->signals.sample_rate())
			{
				beeps_error(
					__FILE__, __LINE__,
					"sample_rate does not match: %f and %f",
					signals->sample_rate(),
					self->signals.sample_rate());
			}

			uint copied_size = Signals_copy(signals, self->signals);
			Signals_shift(&self->signals, copied_size);
			*offset += copied_size;
		}

		if (!self->finished)
		{
			uint fill_size = signals->capacity() - signals->nsamples();
			Signals_fill(signals, fill_size, 0, signals->nsamples());
			*offset += fill_size;
		}
	}


}// Beeps
