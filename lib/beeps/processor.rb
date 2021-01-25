# -*- coding: utf-8 -*-


require 'xot/setter'
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

  end# Processor


  class SineWave

    alias freq= frequency=
    alias freq  frequency

    universal_accessor :frequency, :freq

  end# SineWave


  class SquareWave

    alias freq= frequency=
    alias freq  frequency

    universal_accessor :frequency, :freq

  end# SquareWave


  class SawtoothWave

    alias freq= frequency=
    alias freq  frequency

    universal_accessor :frequency, :freq

  end# SawtoothWave


end# Beeps
