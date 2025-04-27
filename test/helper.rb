%w[../xot ../rucy .]
  .map  {|s| File.expand_path "../#{s}/lib", __dir__}
  .each {|s| $:.unshift s if !$:.include?(s) && File.directory?(s)}

require 'xot/test'
require 'beeps'

require 'test/unit'

include Xot::Test


def get_samples(resolution_per_sec, processor, *a, seconds: 1, **k, &b)
  resolution = resolution_per_sec * seconds
  nsamples   = Beeps.sample_rate  * seconds
  samples    =
    Beeps::Processor.get_signals(processor, *a, seconds: seconds, **k, &b).to_a
  (0..resolution).to_a
    .map {_1 / resolution.to_f}
    .map {(nsamples * _1).to_i}
    .map {_1 == 0 ? 0 : _1 - 1}
    .map {samples[_1]}
    .reverse.drop_while {_1.nil?}.reverse
end
