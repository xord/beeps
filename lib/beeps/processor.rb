# -*- coding: utf-8 -*-


require 'xot/setter'
require 'xot/const_symbol_accessor'
require 'xot/universal_accessor'
require 'xot/block_util'
require 'beeps/ext'


module Beeps


  class Processor

    include Xot::Setter

    def initialize(options = nil, &block)
      super()
      set options if options
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


  class TimeStretch

    def initialize(input = nil, *args, **kwargs, &block)
      super(*args, **kwargs, &block)
      self.input = input
    end

    universal_accessor :scale

  end# TimeStretch


  class PitchShift

    def initialize(input = nil, *args, **kwargs, &block)
      super(*args, **kwargs, &block)
      self.input = input
    end

    universal_accessor :shift

  end# PitchShift


end# Beeps
