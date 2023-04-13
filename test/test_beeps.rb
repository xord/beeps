# -*- coding: utf-8 -*-


require_relative 'helper'


class TestBeeps < Test::Unit::TestCase

  def test_beep()
    assert_nothing_raised {Beeps.be}
    assert_nothing_raised {Beeps.bee}
    assert_nothing_raised {Beeps.beep}
    assert_nothing_raised {Beeps.beeep}
    assert_nothing_raised {Beeps.beeeep}
    assert_nothing_raised {Beeps.beeeeep}
  end

end# TestBeeps
