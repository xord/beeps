require_relative 'helper'


class TestAnalyser < Test::Unit::TestCase

  B = Beeps

  def analyser(...)
    B::Analyser.new(...)
  end

  def osc(...)
    B::Oscillator.new(...)
  end

  def test_initialize()
    assert_equal 1024, analyser.fft_size
  end

  def test_process()
    assert_equal(
      get_samples(1000, osc(:sine)),
      get_samples(1000, osc(:sine) >> analyser))
  end

end# TestAnalyser
