require_relative 'helper'


class TestPitchShift < Test::Unit::TestCase

  B = Beeps

  def shift(...)
    B::PitchShift.new(...)
  end

  def osc(...)
    B::Oscillator.new(...)
  end

  def test_initialize()
    assert_equal 1, shift.shift
  end

  def test_process()
    assert_equal(
      get_samples(10, osc(:sine)),
      get_samples(10, osc(:sine) >> shift(shift: 1)))
  end

  def _test_pitch_shift()
    assert_equal(
      get_samples(1000, osc(:sine, freq: 880)),
      get_samples(1000, osc(:sine, freq: 440) >> shift(shift: 2)))
  end

end# TestPitchShift
