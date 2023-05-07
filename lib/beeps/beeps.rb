require 'beeps/ext'
require 'beeps/sound'
require 'beeps/processor'


module Beeps


  extend module ClassMethods

    def be()
      beep_sound(0.1).play
    end

    def bee()
      beep_sound(0.3).play
    end

    def beep(time = 0.5)
      beep_sound(time).play
    end

    def beeep()
      beep_sound(1).play
    end

    def beeeep()
      beep_sound(1.5).play
    end

    def beeeeep()
      beep_sound(2).play
    end

    def beep_processor=(processor)
      @beep_processor = processor
    end

    private

      def beep_sound(time)
        @beep_processor ||= Oscillator.new(:square)
        Sound.new @beep_processor, time
      end

    self

  end# ClassMethods


end# Beeps
