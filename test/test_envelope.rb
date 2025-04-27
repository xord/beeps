require_relative 'helper'


class TestEnvelope < Test::Unit::TestCase

  B = Beeps

  def envelope(on_sec = 0, off_sec, a: 0, d: 0, s: 1, r: 0)
    B::Envelope.new attack: a, decay: d, sustain: s, release: r do
      note_on   on_sec
      note_off off_sec
    end
  end

  def const(value = 1)
    B::Oscillator.new samples: [value]
  end

  def test_attack()
    o = const(1) >> envelope(1, a: 0.1)
    assert_each_in_delta [0] + [1] * 10, get_samples(10, o)
  end

  def test_decay_sustain()
    o = const(1) >> envelope(1, d: 0.2, s: 0.8)
    assert_each_in_delta [1, 0.9] + [0.8] * 9, get_samples(10, o)
  end

  def test_release()
    o = const(1) >> envelope(0.8, r: 0.1)
    assert_each_in_delta [1] * 9 + [0], get_samples(10, o)
  end

  def test_adsr()
    o = const(1) >> envelope(1, a: 0.1, d: 0.2, s: 0.8, r: 0.4)
    assert_each_in_delta(
      [0, 1, 0.9] + [0.8] * 8 + [0.6, 0.4, 0.2, 0],
      get_samples(10, o, seconds: 2))
  end

  def test_duration()
    o = const(1) >> envelope(0.1)
    assert_each_in_epsilon [1] * 2,  get_samples(10, o, seconds: 2)

    o = const(1) >> envelope(1)
    assert_each_in_epsilon [1] * 11, get_samples(10, o, seconds: 2)

    o = const(1) >> envelope(2)
    assert_each_in_epsilon [1] * 21, get_samples(10, o, seconds: 2)

    o = const(1) >> envelope(3)
    assert_each_in_epsilon [1] * 21, get_samples(10, o, seconds: 2)
  end

end# TestEnvelope
