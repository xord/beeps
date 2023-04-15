# -*- coding: utf-8 -*-


require 'xot/setter'
require 'xot/universal_accessor'
require 'xot/block_util'
require 'beeps/ext'


module Beeps


  class Sound

    def initialize(processor, seconds = 0, nchannels: 1, sample_rate: 0)
      setup processor, seconds, nchannels, sample_rate
    end

    def play(options = nil, &block)
      play!.tap {|player|
        player.set options if options
        Xot::BlockUtil.instance_evan_or_block_call player, &block if block
      end
    end

  end# Sound


end# Beeps
