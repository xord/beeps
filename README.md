# Beeps - Audio synthesis and playback library

[![Ask DeepWiki](https://deepwiki.com/badge.svg)](https://deepwiki.com/xord/beeps)
![License](https://img.shields.io/github/license/xord/beeps)
![Build Status](https://github.com/xord/beeps/actions/workflows/test.yml/badge.svg)
![Gem Version](https://badge.fury.io/rb/beeps.svg)

## ⚠️  Notice

This repository is a read-only mirror of our monorepo.
We do not accept pull requests or direct contributions here.

### 🔄 Where to Contribute?

All development happens in our [xord/all](https://github.com/xord/all) monorepo, which contains all our main libraries.
If you'd like to contribute, please submit your changes there.

For more details, check out our [Contribution Guidelines](./CONTRIBUTING.md).

Thanks for your support! 🙌

## 🚀 About

**Beeps** is an audio library for Ruby. You build a processor graph — oscillators, file / mic input, filters, envelopes, effects — by wiring nodes together with the `>>` operator, then render the result to a `Sound` and play it back.

It is part of the `xord/*` family and underlies the audio support in [Reflex](https://github.com/xord/reflex), [Processing](https://github.com/xord/processing), [RubySketch](https://github.com/xord/rubysketch), and [Reight](https://github.com/xord/reight). Like the rest of the family, it is primarily developed for our own use, but it works as a standalone audio synthesis gem.

## 📋 Requirements

- Ruby **3.0.0** or later
- A C++ compiler with C++20 support
- [Xot](https://rubygems.org/gems/xot) and [Rucy](https://rubygems.org/gems/rucy) (declared as runtime dependencies)
- Platform audio backend:
  - **macOS** — OpenAL and AVFoundation (bundled with the OS)
  - **Windows** — OpenAL (`MINGW_PACKAGE_PREFIX-openal`) and Media Foundation
  - **Linux** — `libopenal-dev`

The following third-party DSP libraries are cloned from GitHub and statically linked while the native extension is being built, so you do not need to install them separately:

| Library                                                                       | Role                                |
| ----------------------------------------------------------------------------- | ----------------------------------- |
| [STK](https://github.com/thestk/stk)                                          | Core synthesis primitives           |
| [AudioFile](https://github.com/adamstark/AudioFile)                           | WAV / AIFF file I/O                 |
| [r8brain-free-src](https://github.com/avaneev/r8brain-free-src)               | High-quality sample-rate conversion |
| [signalsmith-stretch](https://github.com/Signalsmith-Audio/signalsmith-stretch) | Time stretching and pitch shifting  |

## 📦 Installation

Add this line to your Gemfile:
```ruby
gem 'beeps'
```

Then install:
```bash
$ bundle install
```

Or install it directly:
```bash
$ gem install beeps
```

`require 'beeps'` automatically calls `Beeps.init!` and registers `Beeps.fin!` at exit. Set `$BEEPS_NOAUTOINIT = true` before requiring if you want to manage the lifetime yourself.

## 📚 What's Included

### Generators (signal sources)

| Class                | Purpose                                                                  |
| -------------------- | ------------------------------------------------------------------------ |
| `Beeps::Oscillator`  | Sine / triangle / square / sawtooth / noise / sample-playback oscillator |
| `Beeps::Value`       | Constant or linearly-interpolated control value over time                |
| `Beeps::Sequencer`   | Schedule processors at given offsets and durations                       |
| `Beeps::FileIn`      | Stream audio from a WAV / AIFF / other supported file                    |
| `Beeps::MicIn`       | Capture audio from a microphone                                          |
| `Beeps::TextIn`      | Synthesize speech from text                                              |

### Filters (processors that take an input)

| Class                | Purpose                                                                 |
| -------------------- | ----------------------------------------------------------------------- |
| `Beeps::Gain`        | Multiply the signal by a gain coefficient                               |
| `Beeps::Mixer`       | Sum multiple inputs                                                     |
| `Beeps::Envelope`    | Attack / Decay / Sustain / Release envelope with `note_on` / `note_off` |
| `Beeps::LowPass`     | Low-pass filter with cutoff frequency                                   |
| `Beeps::HighPass`    | High-pass filter with cutoff frequency                                  |
| `Beeps::Reverb`      | Reverb with `mix`, `room_size`, `damping` controls                      |
| `Beeps::TimeStretch` | Change duration without affecting pitch                                 |
| `Beeps::PitchShift`  | Change pitch without affecting duration                                 |
| `Beeps::Analyser`    | FFT analyser — exposes time-domain and spectrum readings                |

### Playback

| Class                | Purpose                                                                     |
| -------------------- | --------------------------------------------------------------------------- |
| `Beeps::Sound`       | A finite, renderable audio asset (created from a processor or `load_sound`) |
| `Beeps::SoundPlayer` | Returned by `Sound#play`; supports `pause`, `stop`, `position`, `loop`, ... |

## 💡 Usage

### Play a 440 Hz sine tone for one second

```ruby
require 'beeps'

osc    = Beeps::Oscillator.new(:sine, frequency: 440)
sound  = Beeps::Sound.new(osc, 1)
player = sound.play

sleep 1   # keep the script alive while the sound plays
```

### Build a processor chain with `>>`

```ruby
require 'beeps'

# oscillator -> half-volume -> low-pass at 800 Hz -> reverb
chain =
  Beeps::Oscillator.new(:sawtooth, frequency: 220) >>
  Beeps::Gain.new(0.5) >>
  Beeps::LowPass.new(cutoff: 800) >>
  Beeps::Reverb.new(mix: 0.3, room_size: 0.7)

Beeps::Sound.new(chain, 2).play
sleep 2
```

`a >> b` calls `b.add_input(a)` and returns `b`, so the right-most node is the head of the chain you pass to `Sound.new`.

### ADSR envelope

```ruby
require 'beeps'

osc = Beeps::Oscillator.new(:sine, frequency: 440)
env = Beeps::Envelope.new(0.05, 0.1, 0.7, 0.3, input: osc)
env.note_on
env.note_off 0.5    # release after 0.5 s

Beeps::Sound.new(env, 1).play
sleep 1
```

### Play and save a wave file

```ruby
require 'beeps'

# play an existing file
player = Beeps.load_sound('drum.wav').play

# render a chain and save it to disk
sound = Beeps::Sound.new(Beeps::Oscillator.new(:square, frequency: 880), 0.5)
sound.save 'square.wav'
```

### Capture from the microphone and analyse

```ruby
require 'beeps'

mic      = Beeps::MicIn.new(1)            # 1 channel
analyser = Beeps::Analyser.new(1024, mic) # 1024-point FFT
mic.start

loop do
  sleep 0.1
  puts analyser.spectrum.first(8).map {|v| v.round 3 }.inspect
end
```

## 🛠️ Development

```bash
$ rake lib    # build the native C++ library (libbeeps)
$ rake ext    # build the Ruby C extension
$ rake test   # run the test suite
$ rake doc    # generate RDoc from C++ sources
$ rake       # default: builds the extension
```

In the [`xord/all`](https://github.com/xord/all) monorepo you can scope by module, e.g. `rake beeps test`.

## 📜 License

**Beeps** is licensed under the MIT License.
See the [LICENSE](./LICENSE) file for details.

The third-party DSP libraries listed above retain their own licenses.
