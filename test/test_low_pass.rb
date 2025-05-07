require_relative 'helper'


class TestLowPass < Test::Unit::TestCase

  B = Beeps

  def lpass(...)
    B::LowPass.new(...)
  end

  def test_initialize()
    assert_equal 1000, lpass   .cutoff
    assert_equal 1,    lpass(1).cutoff
  end

  def test_cutoff_freqneucy()
    f = lpass    1
    assert_equal 1, f.cutoff_frequency
    assert_equal 1, f.cutoff

    f.cutoff_frequency = 2
    assert_equal         2, f.cutoff_frequency
    assert_equal         2, f.cutoff

    f.cutoff_frequency   3
    assert_equal         3, f.cutoff_frequency
    assert_equal         3, f.cutoff

    f.cutoff =   4
    assert_equal 4, f.cutoff_frequency
    assert_equal 4, f.cutoff

    f.cutoff     5
    assert_equal 5, f.cutoff_frequency
    assert_equal 5, f.cutoff

    assert_raise(ArgumentError) {f.cutoff 0}
  end

end# TestLowPass
