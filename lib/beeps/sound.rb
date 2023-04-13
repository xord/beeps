# -*- coding: utf-8 -*-


require 'beeps/ext'


module Beeps


  class Sound

    def initialize(processor, seconds = 0, nchannels: 1, sample_rate: 0)
      setup processor, seconds, nchannels, sample_rate
    end

    def self.load(path)
      f = FileIn.new path
      Sound.new f, f.seconds, nchannels: f.nchannels
    end

  end# Sound


end# Beeps
