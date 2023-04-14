# -*- coding: utf-8 -*-


require 'beeps/ext'


module Beeps


  class Sound

    def initialize(processor, seconds = 0, nchannels: 1, sample_rate: 0)
      setup processor, seconds, nchannels, sample_rate
    end

  end# Sound


end# Beeps
