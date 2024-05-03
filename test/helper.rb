%w[../xot ../rucy .]
  .map  {|s| File.expand_path "../#{s}/lib", __dir__}
  .each {|s| $:.unshift s if !$:.include?(s) && File.directory?(s)}

p [10, Time.now.to_f]
require 'xot/test'
p [11, Time.now.to_f]
require 'beeps'
p [12, Time.now.to_f]

require 'test/unit'
p [13, Time.now.to_f]

include Xot::Test
p [14, Time.now.to_f]


unless defined?($BEEPS_NOAUTOINIT) && $BEEPS_NOAUTOINIT
  #def Beeps.fin!() end
end
