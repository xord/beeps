require_relative 'helper'


class TestProcessor < Test::Unit::TestCase

  B = Beeps
  P = B::Processor

  def processor()
    B::Gain.new
  end

  def test_initialize()
    assert_raise(Beeps::BeepsError) {B::Processor.new}
  end

  def test_input()
    p = processor
    assert_nil p.input

    p.input = B::Oscillator.new
    assert_equal B::Oscillator, p.input.class

    p.input nil
    assert_nil p.input
  end

  def test_shift_right()
    osc   = B::Oscillator.new
    gain1 = B::Gain.new
    gain2 = B::Gain.new
    assert_equal gain2, (osc >> gain1 >> gain2)
    assert_equal nil,   osc.input
    assert_equal osc,   gain1.input
    assert_equal gain1, gain2.input
  end

  def _test_shift_left()
    osc   = B::Oscillator.new
    gain1 = B::Gain.new
    gain2 = B::Gain.new
    assert_equal osc, (gain2 << gain1 << osc)
    assert_equal nil,   osc.input
    assert_equal osc,   gain1.input
    assert_equal gain1, gain2.input
  end

  def test_get_signals()
    assert_equal 4000, P.get_signals(
      B::Oscillator.new, seconds: 2, nchannels: 2, sample_rate: 1000
    ).to_a.size
  end

end# TestProcessor
