require_relative 'helper'


class TestMixer < Test::Unit::TestCase

  B = Beeps

  def mixer(*inputs)
    B::Mixer.new.tap {_1.add_input(*inputs)}
  end

  def const(value = 1)
    B::Oscillator.new samples: [value]
  end

  def envelope(note_off_seconds, a = 0, r = 0)
    B::Envelope.new attack: a, decay: 0, sustain: 1, release: r do
      note_on
      note_off note_off_seconds
    end
  end

  def test_inputs()
    m = mixer const(0.1), const(0.2)
    assert_equal 2, m.inputs.size
  end

  def test_process()
    m = mixer const(0.1), const(0.2)
    assert_each_in_epsilon [0.3] * 4, get_samples(3, m)
  end

  def test_mix_difference_duration()
    mixer(
      const(0.1) >> envelope(1),
      const(0.2) >> envelope(1)
    ).tap do |m|
      assert_each_in_epsilon [0.3] * 11,             get_samples(10, m)
    end

    mixer(
      const(0.1) >> envelope(0.1),
      const(0.2) >> envelope(0.5)
    ).tap do |m|
      assert_each_in_epsilon [0.3] * 2 + [0.2] * 4,  get_samples(10, m)
    end

    mixer(
      const(0.1) >> envelope(0.5),
      const(0.2) >> envelope(0.1)
    ).tap do |m|
      assert_each_in_epsilon [0.3] * 2 + [0.1] * 4,  get_samples(10, m)
    end

    mixer(
      const(0.1) >> envelope(0.5),
      const(0.2) >> envelope(1.5)
    ).tap do |m|
      assert_each_in_epsilon [0.3] * 6 + [0.2] * 10, get_samples(10, m, seconds: 2)
    end
  end

end# TestMixer
