# -*- coding: utf-8 -*-


require_relative 'helper'


class TestSoundPlayer < Test::Unit::TestCase

  B = Beeps
  S = B::Sound

  PATH = 'test.wav'

  def sound(seconds = 0.1, processor: B::Oscillator.new, **kwargs)
    S.new processor, seconds, nchannels: 2, **kwargs
  end

  def stream_sound()
    sound 0, processor: B::FileIn.new(PATH)
  end

  def setup()
    sound.save PATH
  end

  def teardown()
    B::SoundPlayer.stop_all
    File.delete PATH if File.exist?(PATH)
  end

  def test_pause()
    p = sound.play
    assert_equal [true,  false, false], [p.playing?, p.paused?, p.stopped?]
    p.pause
    assert_equal [false, true,  false], [p.playing?, p.paused?, p.stopped?]

    p = stream_sound.play
    assert_equal [true,  false, false], [p.playing?, p.paused?, p.stopped?]
    p.pause
    assert_equal [false, true,  false], [p.playing?, p.paused?, p.stopped?]
  end

  def test_stop()
    p = sound.play
    assert_equal [true,  false, false], [p.playing?, p.paused?, p.stopped?]
    p.stop
    assert_equal [false, false, true],  [p.playing?, p.paused?, p.stopped?]

    p = stream_sound.play
    assert_equal [true,  false, false], [p.playing?, p.paused?, p.stopped?]
    p.stop
    assert_equal [false, false, true],  [p.playing?, p.paused?, p.stopped?]
  end

  def test_play_end_then_stop()
    s   = sound
    sec = s.seconds

    p = s.play
    assert_equal [true,  false, false], [p.playing?, p.paused?, p.stopped?]
    sleep sec * 2
    assert_equal [false, false, true],  [p.playing?, p.paused?, p.stopped?]

    s = stream_sound
    p = s.play
    assert_equal [true,  false, false], [p.playing?, p.paused?, p.stopped?]
    sleep sec * 2
    assert_equal [false, false, true],  [p.playing?, p.paused?, p.stopped?]
  end

  def test_play_after_pause()
    p = sound.play
    p.pause
    assert_equal [false, true,  false], [p.playing?, p.paused?, p.stopped?]
    p.play
    assert_equal [true,  false, false], [p.playing?, p.paused?, p.stopped?]

    p = stream_sound.play
    p.pause
    assert_equal [false, true,  false], [p.playing?, p.paused?, p.stopped?]
    p.play
    assert_equal [true,  false, false], [p.playing?, p.paused?, p.stopped?]
  end

  def test_stop_after_pause()
    p = sound.play
    p.pause
    assert_equal [false, true,  false], [p.playing?, p.paused?, p.stopped?]
    p.stop
    assert_equal [false, false, true],  [p.playing?, p.paused?, p.stopped?]

    p = stream_sound.play
    p.pause
    assert_equal [false, true,  false], [p.playing?, p.paused?, p.stopped?]
    p.stop
    assert_equal [false, false, true],  [p.playing?, p.paused?, p.stopped?]
  end

  def test_stop_all()
    s = sound
    players = 10.times.map {s.play}
    assert_true players.all? {|p| p.playing?}

    B::SoundPlayer.stop_all
    assert_true players.all? {|p| p.stopped?}
  end

end# TestSoundPlayer
