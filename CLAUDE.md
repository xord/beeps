# Beeps

Audio synthesis and playback library. Provides an audio processing chain with oscillators, filters, envelopes, and effects.

## Processor Chain

Connect processors with the `>>` operator:
```ruby
Beeps::Oscillator.new >> Beeps::Gain.new(gain: 0.5)
```

## External Libraries

Automatically fetched and statically linked at build time:
- STK 5.0.1 — DSP toolkit
- AudioFile 1.1.1 — Audio file I/O
- r8brain-free-src 6.2 — Sample rate conversion
- signalsmith-stretch — Time stretching

## Platform Dependencies

- macOS: OpenAL, AVFoundation
- Windows: OpenAL, Media Foundation
- Linux: libopenal-dev
