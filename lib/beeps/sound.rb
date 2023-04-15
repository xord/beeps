# -*- coding: utf-8 -*-


require 'xot/setter'
require 'xot/universal_accessor'
require 'xot/block_util'
require 'beeps/ext'


module Beeps


  class Sound

    def initialize(processor, seconds = 0, nchannels: 1, sample_rate: 0, &block)
      setup processor, seconds, nchannels, sample_rate
      Xot::BlockUtil.instance_evan_or_block_call self, &block if block
    end

    def play(options = nil, &block)
      play!.tap {|player|
        player.set options if options
        Xot::BlockUtil.instance_evan_or_block_call player, &block if block
      end
    end

  end# Sound


end# Beeps
