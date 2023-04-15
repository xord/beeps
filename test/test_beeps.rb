# -*- coding: utf-8 -*-


require_relative 'helper'


class TestBeeps < Test::Unit::TestCase

  B = Beeps

  def test_beep()
    B.beep_processor = B::Oscillator.new >> B::Gain.new(gain: 0)
    assert_nothing_raised {B.be}
    assert_nothing_raised {B.bee}
    assert_nothing_raised {B.beep}
    assert_nothing_raised {B.beeep}
    assert_nothing_raised {B.beeeep}
    assert_nothing_raised {B.beeeeep}
  end

end# TestBeeps
