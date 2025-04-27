require_relative 'helper'


class TestTimeStretch < Test::Unit::TestCase

  B = Beeps

  def stretch(...)
    B::TimeStretch.new(...)
  end

  def osc(...)
    B::Oscillator.new(...)
  end

  def env(off_time = 1, *a, **k)
    B::Envelope.new(*a, **k) {note_off off_time}
  end

  def test_initialize()
    assert_equal 1, stretch.scale
  end

  def test_process()
    assert_equal(
      get_samples(10, osc(:sine)),
      get_samples(10, osc(:sine) >> stretch(scale: 1)))
  end

  def test_time_stretch()
    assert_equal 11, get_samples(10, osc(:sine) >> env >> stretch(scale: 1))  .size
    assert_equal 6,  get_samples(10, osc(:sine) >> env >> stretch(scale: 0.5)).size
    assert_equal 2,  get_samples(10, osc(:sine) >> env >> stretch(scale: 0.1)).size
  end

end# TestTimeStretch
