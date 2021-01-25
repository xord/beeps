# -*- coding: utf-8 -*-


require 'beeps/ext'


module Beeps


  class Sound

    def self.load(path)
      Sound.new FileIn.new(path), 1
    end

  end# Sound


end# Beeps
