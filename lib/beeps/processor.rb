# -*- coding: utf-8 -*-


require 'xot/setter'
require 'xot/const_symbol_accessor'
require 'xot/universal_accessor'
require 'xot/block_util'
require 'beeps/ext'


module Beeps


  class Processor

    include Xot::Setter

    def initialize(**options, &block)
      super()
      set options unless options.empty?
      Xot::BlockUtil.instance_eval_or_block_call self, &block if block
    end

    def >>(o)
      o.input = self
      o
    end

    def <<(o)
      self.input = o
      o
    end

    universal_accessor :input

  end# Processor


  class Oscillator

    const_symbol_accessor :type, **{
      none:     NONE,
      sine:     SINE,
      triangle: TRIANGLE,
      square:   SQUARE,
      sawtooth: SAWTOOTH
    }

    def initialize(type = :sine, *args, **kwargs, &block)
      super(*args, **kwargs, &block)
      self.type = type
    end

    alias freq= frequency=
    alias freq  frequency

    universal_accessor :type, :frequency, :freq

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


  class ADSR

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

  end# ADSR


  class TimeStretch

    universal_accessor :scale

  end# TimeStretch


  class PitchShift

    universal_accessor :shift

  end# PitchShift


  class Analyser

    def each_signal(nsamples = fft_size, &block)
      return enum_for(:each_signal, nsamples) unless block
      each_signal!(nsamples, &block)
    end

    def each_spectrum(&block)
      return enum_for(:each_spectrum) unless block
      each_spectrum!(&block)
    end

    universal_accessor :fft_size

  end# Analyser


end# Beeps
