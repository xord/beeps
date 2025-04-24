require_relative 'helper'


class TestSignals < Test::Unit::TestCase

  B = Beeps

  def signals(**kwargs)
    B::Processor.get_signals B::Oscillator.new, **kwargs
  end

  def test_initialize()
    assert_true B::Signals.new.empty?
  end

  def test_dup()
    sig = signals nchannels: 10, sample_rate: 20
    assert_equal 10,                sig.dup.nchannels
    assert_equal 20,                sig.dup.sample_rate
    assert_not_equal sig.object_id, sig.dup.object_id
  end

  def test_sample_rate()
    assert_equal 44100, signals                  .sample_rate
    assert_equal   100, signals(sample_rate: 100).sample_rate
  end

  def test_nchannels()
    assert_equal 1, signals              .nchannels
    assert_equal 2, signals(nchannels: 2).nchannels
  end

  def test_nsamples()
    assert_equal 44100, signals(seconds: 1)                    .nsamples
    assert_equal 88200, signals(seconds: 2)                    .nsamples
    assert_equal   100, signals(seconds: 1,   sample_rate: 100).nsamples
    assert_equal   200, signals(seconds: 2,   sample_rate: 100).nsamples
    assert_equal    50, signals(seconds: 0.5, sample_rate: 100).nsamples

    assert_equal   100, signals(seconds: 1, nchannels: 1, sample_rate: 100).nsamples
    assert_equal   200, signals(seconds: 1, nchannels: 2, sample_rate: 100).nsamples
  end

  def test_each()
    assert_equal 100, signals(nchannels: 1, sample_rate: 100).each            .to_a.size
    assert_equal 200, signals(seconds: 2,   sample_rate: 100).each            .to_a.size
    assert_equal 200, signals(nchannels: 2, sample_rate: 100).each            .to_a.size
    assert_equal 100, signals(nchannels: 2, sample_rate: 100).each(channel: 0).to_a.size
    assert_equal 100, signals(nchannels: 2, sample_rate: 100).each(channel: 1).to_a.size

    assert_raise(ArgumentError) {signals(nchannels: 2).each(channel: -1).to_a}
    assert_raise(ArgumentError) {signals(nchannels: 2).each(channel: 2) .to_a}

    assert_in_delta 0, signals(sample_rate: 100).each.to_a.then {_1.sum / _1.size}, 0.001
  end

end# TestSignals
