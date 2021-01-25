# -*- coding: utf-8 -*-


require_relative 'helper'


class TestSound < Test::Unit::TestCase

  def s()
    Beeps::Sound.new Beeps::SineWave.new, 0.1
  end

  def test_play()
    assert_nothing_raised {s.play}
  end

  def test_beep()
    assert_nothing_raised {Beeps.be}
    assert_nothing_raised {Beeps.bee}
    assert_nothing_raised {Beeps.beep}
    assert_nothing_raised {Beeps.beeep}
    assert_nothing_raised {Beeps.beeeep}
    assert_nothing_raised {Beeps.beeeeep}
  end

end# TestBeeps
