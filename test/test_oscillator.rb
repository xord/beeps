require_relative 'helper'


class TestOscillator < Test::Unit::TestCase

  B = Beeps

  def osc(type = :sine, samples: nil, freq: 1)
    B::Oscillator.new(type, samples: samples, freq: freq)
  end

  def test_sine_wave()
    assert_in_delta 0, get_samples(1000, osc(:sine)).sum / B.sample_rate
  end

  def _test_triangle_wave()
    assert_in_delta 0, get_sig(osc :triangle).sum / B.sample_rate
  end

  def test_samples_wave()
    assert_each_in_delta [1] * 1001, get_samples(1000, osc(samples: [1]))
    assert_each_in_delta [1, 1, 1],  get_samples(2,    osc(samples: [1]))
    assert_each_in_delta [1, 0, 1],  get_samples(2,    osc(samples: [1, 0])), 0.005
  end

end# TestOscillator
