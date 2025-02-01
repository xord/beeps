require 'xot/setter'
require 'xot/const_symbol_accessor'
require 'xot/universal_accessor'
require 'xot/hookable'
require 'xot/block_util'
require 'beeps/ext'


module Beeps


  class Processor

    include Xot::Setter
    include Xot::Hookable

    def initialize(**options, &block)
      super()
      set options unless options.empty?
      Xot::BlockUtil.instance_eval_or_block_call self, &block if block
    end

    universal_accessor :input

    def >>(o)
      o.input = self
      o
    end

    def <<(o)
      self.input = o
      o
    end

  end# Processor


  class Oscillator

    include Enumerable

    const_symbol_accessor :type, **{
      none:     TYPE_NONE,
      sine:     SINE,
      triangle: TRIANGLE,
      square:   SQUARE,
      sawtooth: SAWTOOTH,
      samples:  SAMPLES
    }

    def initialize(type = :sine, *args, samples: nil, **kwargs, &block)
      super(*args, **kwargs, &block)
      if samples
        self.samples = samples
      else
        self.type = type
      end
    end

    universal_accessor :type, :frequency, :phase

    alias freq= frequency=
    alias freq  frequency

    def each_sample(&block)
      block ? each_sample!(&block) : enum_for(:each_sample!)
    end

    alias each each_sample

  end# Oscillator


  class FileIn

    def initialize(path = nil, *args, **kwargs, &block)
      super(*args, **kwargs, &block)
      self.path = path if path
    end

    universal_accessor :path

  end# FileIn


  class Gain

    universal_accessor :gain

  end# Gain


  class Envelope

    def initialize(
      attack = nil, decay = nil, sustain = nil, release = nil,
      *args, **kwargs, &block)

      attack_time   attack  if attack
      decay_time    decay   if decay
      sustain_level sustain if sustain
      release_time  release if release

      super(*args, **kwargs, &block)
    end

    def note_on(delay = 0)
      note_on! delay
    end

    def note_off(delay = 0)
      note_off! delay
    end

    universal_accessor :attack_time, :decay_time, :sustain_level, :release_time

    alias  attack=  attack_time=
    alias  attack   attack_time
    alias   decay=   decay_time=
    alias   decay    decay_time
    alias sustain= sustain_level=
    alias sustain  sustain_level
    alias release= release_time=
    alias release  release_time

  end# Envelope


  class TimeStretch

    universal_accessor :scale

  end# TimeStretch


  class PitchShift

    universal_accessor :shift

  end# PitchShift


  class Analyser

    universal_accessor :fft_size

    def each_signal(nsamples = fft_size, &block)
      return enum_for(:each_signal, nsamples) unless block
      each_signal!(nsamples, &block)
    end

    def each_spectrum(&block)
      return enum_for(:each_spectrum) unless block
      each_spectrum!(&block)
    end

  end# Analyser


end# Beeps
