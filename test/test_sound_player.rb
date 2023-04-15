# -*- coding: utf-8 -*-


require_relative 'helper'


class TestSoundPlayer < Test::Unit::TestCase

  B = Beeps

  PATH = 'test.wav'

  def sound(seconds = 0.1, processor: B::Oscillator.new, **kwargs)
    B::Sound.new processor >> B::Gain.new(gain: 0), seconds, **kwargs
  end

  def stream_sound(**kwargs)
    sound 0, processor: B::FileIn.new(PATH), **kwargs
  end

  def setup()
    sound.save PATH
  end

  def teardown()
    B::SoundPlayer.stop_all
    File.delete PATH if File.exist?(PATH)
  end

  def test_initialize()
    assert_in_epsilon 1, sound.play.gain
    assert_false         sound.play.loop
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

  def test_gain()
    p = sound.play
    assert_in_epsilon 1,   p.gain

    p.gain = 0.1
    assert_in_epsilon 0.1, p.gain

    p.gain 0.2
    assert_in_epsilon 0.2, p.gain
  end

  def test_loop()
    p = sound.play
    assert_false p.loop

    p.loop = true
    assert_true  p.loop

    p.loop false
    assert_false p.loop
  end

  def test_stop_all()
    s = sound
    players = 10.times.map {s.play}
    assert_true players.all? {|p| p.playing?}

    B::SoundPlayer.stop_all
    assert_true players.all? {|p| p.stopped?}
  end

end# TestSoundPlayer
