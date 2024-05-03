#require_relative 'helper'


class TestSoundPlayer < Test::Unit::TestCase

  B = Beeps

  PATH = 'test.wav'

  def sound(seconds = 0.1, processor: B::Oscillator.new, **kwargs)
    B::Sound.new processor >> B::Gain.new(gain: 0), seconds, **kwargs
  end
=begin
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
    assert_equal :playing, p.state
    p.pause
    assert_equal :paused,  p.state

    p = stream_sound.play
    assert_equal :playing, p.state
    p.pause
    assert_equal :paused,  p.state
  end

  def test_stop()
    p = sound.play
    assert_equal :playing, p.state
    p.stop
    assert_equal :stopped, p.state

    p = stream_sound.play
    assert_equal :playing, p.state
    p.stop
    assert_equal :stopped, p.state
  end
=end
  def _test_play_end_then_stop()
pp [1, Time.now.to_f]
    s   = sound
pp [2, Time.now.to_f]
    sec = s.seconds
pp [3, Time.now.to_f]
    p(sec:)
    p(method(:sleep))

pp [4, Time.now.to_f]
    p = s.play
pp [5, Time.now.to_f]
    assert_equal :playing, p.state
pp [6, Time.now.to_f]
    sleep sec * 10
pp [7, Time.now.to_f]
    assert_equal :stopped, p.state
pp [8, Time.now.to_f]

    s = stream_sound
    p = s.play
    assert_equal :playing, p.state
    sleep sec * 2
    assert_equal :stopped, p.state
  end
=begin
  def test_play_after_pause()
    p = sound.play
    p.pause
    assert_equal :paused,  p.state
    p.play
    assert_equal :playing, p.state

    p = stream_sound.play
    p.pause
    assert_equal :paused,  p.state
    p.play
    assert_equal :playing, p.state
  end

  def test_stop_after_pause()
    p = sound.play
    p.pause
    assert_equal :paused,  p.state
    p.stop
    assert_equal :stopped, p.state

    p = stream_sound.play
    p.pause
    assert_equal :paused,  p.state
    p.stop
    assert_equal :stopped, p.state
  end

  def test_state()
    p = sound.play
    assert_equal :playing, p.state
    assert_equal [true,  false, false], [p.playing?, p.paused?, p.stopped?]

    p.pause
    assert_equal :paused,  p.state
    assert_equal [false, true,  false], [p.playing?, p.paused?, p.stopped?]

    p.stop
    assert_equal :stopped, p.state
    assert_equal [false, false, true],  [p.playing?, p.paused?, p.stopped?]

    p = stream_sound.play
    assert_equal :playing, p.state
    assert_equal [true,  false, false], [p.playing?, p.paused?, p.stopped?]

    p.pause
    assert_equal :paused,  p.state
    assert_equal [false, true,  false], [p.playing?, p.paused?, p.stopped?]

    p.stop
    assert_equal :stopped, p.state
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
=end
end# TestSoundPlayer
