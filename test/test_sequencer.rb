require_relative 'helper'


class TestSequencer < Test::Unit::TestCase

  B = Beeps

  def seq(&b)
    B::Sequencer.new.tap {b.call _1}
  end

  def osc(type = :sine, samples: nil, freq: 1, **kwargs)
    B::Oscillator.new(type, samples: samples, freq: freq, **kwargs)
  end

  def test_each_note()
    assert_equal(
      [[B::Oscillator,  0, 1]],
      seq {_1.add osc, 0, 1}.map {|p, *args| [p.class, *args]})

    assert_equal [0],    seq {_1.add osc, 0, 1}                  .map {_2}
    assert_equal [0, 2], seq {_1.add osc, 0, 1; _1.add osc, 2, 3}.map {_2}
  end

end# TestSequencer
