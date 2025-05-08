require_relative 'helper'


class TestOscillator < Test::Unit::TestCase

  B = Beeps

  def osc(type = :sine, samples: nil, freq: 1, **kwargs)
    B::Oscillator.new(type, samples: samples, freq: freq, **kwargs)
  end

  def test_initialize()
    assert_equal      1, osc(frequency: 1).frequency
    assert_equal      2, osc(frequency: 2).freq
    assert_equal      3, osc(freq:      3).frequency
    assert_equal      4, osc(freq:      4).freq
    assert_in_delta 0.5, osc(phase:   5.5).phase
    assert_equal      6, osc(gain:      6).gain
    assert_equal      7, osc(offset:    7).offset
    assert_in_delta 0.8, osc(duty:    0.8).duty
  end

  def test_offset()
    assert_in_delta  0, get_samples(10000, osc(offset:  0)).sum / 10000
    assert_in_delta  3, get_samples(10000, osc(offset:  3)).sum / 10000
    assert_in_delta -3, get_samples(10000, osc(offset: -3)).sum / 10000
  end

  def test_gain()
    assert_in_delta  3, get_samples(100, osc(gain: 3)).max
    assert_in_delta -3, get_samples(100, osc(gain: 3)).min
  end

  def test_offset_and_gain()
    assert_in_delta 13, get_samples(100, osc(offset: 10, gain: 3)).max
    assert_in_delta  7, get_samples(100, osc(offset: 10, gain: 3)).min
  end

  def test_waves()
    assert_in_delta 0, get_samples(10000, osc(:sine))    .sum / 10000
    assert_in_delta 0, get_samples(10000, osc(:triangle)).sum / 10000
    assert_in_delta 0, get_samples(10000, osc(:square))  .sum / 10000
    assert_in_delta 0, get_samples(10000, osc(:sawtooth)).sum / 10000, 0.05
  end

  def test_square_with_duty()
    assert_not_equal(
      get_samples(1000, osc(:square, duty: 0.1)),
      get_samples(1000, osc(:square, duty: 0.2)))
  end

  def test_samples_wave()
    assert_each_in_delta [1] * 1001, get_samples(1000, osc(samples: [1]))
    assert_each_in_delta [1, 1, 1],  get_samples(2,    osc(samples: [1]))
    assert_each_in_delta [1, 0, 1],  get_samples(2,    osc(samples: [1, 0])), 0.005
  end

end# TestOscillator
