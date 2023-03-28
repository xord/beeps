# -*- coding: utf-8 -*-


require 'beeps/ext'


module Beeps


  class Sound

    def self.load(path)
      f = FileIn.new path
      Sound.new f, f.seconds, f.nchannels
    end

  end# Sound


end# Beeps
