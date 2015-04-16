# -*- coding: utf-8 -*-


require 'beeps/ext'


module Beeps


  class Sound

    def self.load (path)
      Sound.new FileInput.new(path), 1
    end

  end# Sound


end# Beeps
