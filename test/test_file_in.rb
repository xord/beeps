require 'tempfile'
require_relative 'helper'


class TestFileIn < Test::Unit::TestCase

  B = Beeps

  def filein(path)
    B::FileIn.new path
  end

  def osc(...)
    B::Oscillator.new(...)
  end

  def test_load_file()
    Tempfile.create 'tmp.wav' do |file|
      file.close

      B::Sound.new(osc(:sine), 3).save(file.path)

      assert_each_in_delta(
        get_samples(100, osc(:sine)),
        get_samples(100, filein(file.path)))
    end
  end

  def test_file_not_found()
    assert_raise(B::BeepsError) {B::FileIn.new 'nofile.wav'}
  end

end# TestFileIn
