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

  def wav(tag, bits, rate, data)
    align = bits / 8
    fmt   = [tag, 1, rate, rate * align, align, bits].pack('vvVVvv')
    ['RIFF', 36 + data.bytesize, 'WAVE', 'fmt ', 16].pack('a4Va4a4V') + fmt +
      ['data', data.bytesize].pack('a4V') + data
  end

  def save_wav(filename, *wav_args, &block)
    Tempfile.create ["#{filename}", '.wav'] do |file|
      file.binmode
      file.write wav(*wav_args)
      file.close
      block.call file.path
    end
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

  def test_load_formats()
    rate   = B.sample_rate
    values = (0...rate / 10).map {Math.sin(2 * Math::PI * _1 / 100) * 0.5}
    {
      pcm8:    [1, 8,  0.01,   values.map {(_1 * 128 + 128).round}.pack('C*')],
      pcm16:   [1, 16, 0.0001, values.map {(_1 * 32768).round}.pack('s<*')],
      pcm24:   [1, 24, 0.0001, values.map {(_1 * 8388608).round}
                                     .flat_map {[_1, _1 >> 8, _1 >> 16]}
                                     .map {_1 & 0xff}.pack('C*')],
      pcm32:   [1, 32, 0.0001, values.map {(_1 * 2147483648).round}.pack('l<*')],
      float32: [3, 32, 0.0001, values.pack('e*')]
    }.each do |name, (tag, bits, delta, data)|
      save_wav name, tag, bits, rate, data do |path|
        samples = B::Processor
          .get_signals(filein(path), seconds: values.size.to_f / rate)
          .to_a
        assert_equal values.size, samples.size, name
        assert_each_in_delta values, samples, delta, name
      end
    end
  end

  def test_file_not_found()
    assert_raise(B::BeepsError) {B::FileIn.new 'nofile.wav'}
  end

end# TestFileIn
