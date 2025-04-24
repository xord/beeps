require 'beeps/ext'


module Beeps


  class Signals

    include Enumerable

    def each(channel: nil, &block)
      return enum_for(:each, channel: channel) unless block
      each! channel, &block
    end

  end# Signals


end# Beeps
