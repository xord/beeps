# -*- coding: utf-8 -*-


require_relative 'helper'


class TestSoundPlayer < Test::Unit::TestCase

  B = Beeps
  S = B::Sound

  PATH = 'test.wav'

  def sound(seconds = 0.1)
    S.new B::Oscillator.new, seconds, nchannels: 2
  end

  def teardown()
    File.delete PATH if File.exist?(PATH)
  end

  def test_pause()
    p = sound.play
    assert_equal [true, false, false], [p.playing?, p.paused?, p.stopped?]
    p.pause
    assert_equal [false, true, false], [p.playing?, p.paused?, p.stopped?]
  end

  def test_stop()
    p = sound.play
    assert_equal [true, false, false], [p.playing?, p.paused?, p.stopped?]
    p.stop
    assert_equal [false, false, true], [p.playing?, p.paused?, p.stopped?]
  end

  def test_play_pause_stop_state()
    s = sound
    p = s.play
    assert_equal [true, false, false], [p.playing?, p.paused?, p.stopped?]
    p.pause
    assert_equal [false, true, false], [p.playing?, p.paused?, p.stopped?]
    p.play
    assert_equal [true, false, false], [p.playing?, p.paused?, p.stopped?]
    sleep s.seconds * 2
    assert_equal [false, false, true], [p.playing?, p.paused?, p.stopped?]
  end

end# TestSoundPlayer
