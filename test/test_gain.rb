require_relative 'helper'


class TestGain < Test::Unit::TestCase

  B = Beeps

  def gain(...)
    B::Gain.new(...)
  end

  def const(value = 1)
    B::Oscillator.new samples: [value]
  end

  def test_initialize()
    assert_equal 1, gain   .gain
    assert_equal 2, gain(2).gain
  end

  def test_process()
    assert_equal [2]   * 4, get_samples(3, const(1) >> gain(2))
    assert_equal [0.5] * 4, get_samples(3, const(1) >> gain(0.5))
    assert_equal [0]   * 4, get_samples(3, const(1) >> gain(0))
    assert_equal [-1]  * 4, get_samples(3, const(1) >> gain(-1))
  end

end# TestGain
