# -*- coding: utf-8 -*-


require 'xot/setter'
require 'xot/block_util'
require 'beeps/ext'


module Beeps


  class SineWave

    include Xot::Setter

    alias freq= frequency=
    alias freq  frequency

    def initialize (opts = {}, &block)
      super()
      set opts
      Xot::BlockUtil.instance_eval_or_block_call self, &block if block
    end

  end# SineWave


  class SquareWave

    include Xot::Setter

    alias freq= frequency=
    alias freq  frequency

    def initialize (opts = {}, &block)
      super()
      set opts
      Xot::BlockUtil.instance_eval_or_block_call self, &block if block
    end

  end# SquareWave


  class SawtoothWave

    include Xot::Setter

    alias freq= frequency=
    alias freq  frequency

    def initialize (opts = {}, &block)
      super()
      set opts
      Xot::BlockUtil.instance_eval_or_block_call self, &block if block
    end

  end# SawtoothWave


end# Beeps
