require_relative 'helper'


class TestHighPass < Test::Unit::TestCase

  B = Beeps

  def hpass(...)
    B::HighPass.new(...)
  end

  def test_initialize()
    assert_equal 100, hpass   .cutoff
    assert_equal 1,   hpass(1).cutoff
  end

  def test_cutoff_freqneucy()
    f = hpass    1
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

end# TestHighPass
