require 'xot/setter'
require 'xot/universal_accessor'
require 'xot/block_util'
require 'beeps/ext'


module Beeps


  class Sound

    include Xot::Setter

    def initialize(
      processor, seconds = 0, nchannels: 1, sample_rate: 0, **options, &block)

      setup processor, seconds, nchannels, sample_rate
      set(**options) unless options.empty?
      Xot::BlockUtil.instance_eval_or_block_call self, &block if block
    end

    def play(**options, &block)
      play!.tap do |player|
        player.set(**options) unless options.empty?
        Xot::BlockUtil.instance_eval_or_block_call player, &block if block
      end
    end

    universal_accessor :gain, :loop

  end# Sound


  class SoundPlayer

    include Xot::Setter

    universal_accessor :gain, :loop

  end# SoundPlayer


end# Beeps
