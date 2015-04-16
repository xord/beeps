# -*- coding: utf-8 -*-


require_relative 'helper'


class TestSound < Test::Unit::TestCase

  def s ()
    Beeps::Sound.load '/Users/snori/Desktop/test.wav'
  end

  def test_play ()
    assert_nothing_raised {s.play}
  end

end# TestBeeps
