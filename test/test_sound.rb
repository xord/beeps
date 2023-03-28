# -*- coding: utf-8 -*-


require_relative 'helper'


class TestSound < Test::Unit::TestCase

  def s()
    Beeps::Sound.new Beeps::Oscillator.new, 0.1
  end

  def test_play()
    assert_nothing_raised {s.play}
  end

end# TestBeeps
