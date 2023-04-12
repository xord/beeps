# -*- coding: utf-8 -*-


require_relative 'helper'


class TestSound < Test::Unit::TestCase

  B = Beeps
  S = B::Sound

  PATH = 'test.wav'

  def sound(seconds = 0.1)
    S.new B::Oscillator.new, seconds, nchannels: 2
  end

  def teardown()
    File.delete PATH if File.exist?(PATH)
  end

  def test_play()
    assert_nothing_raised {sound.play}
  end

  def test_save()
    assert_equal false, File.exist?(PATH)
    assert_nothing_raised {sound.save PATH}
    assert_equal true,  File.exist?(PATH)
    assert_nothing_raised {S.load PATH}
  end

  def test_load()
    assert_nothing_raised {sound(0.2).save PATH}

    s = S.load PATH
    assert_in_epsilon 44100, s.sample_rate
    assert_equal      2,     s.nchannels
    assert_in_epsilon 0.2,   s.seconds
    assert_nothing_raised {s.play}
  end

end# TestBeeps
