require_relative 'helper'


class TestFileIn < Test::Unit::TestCase

  B = Beeps

  def test_initialize()
    assert_raise(B::BeepsError) {B::FileIn.new 'nofile.wav'}
  end

end# TestFileIn
