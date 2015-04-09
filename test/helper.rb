# -*- coding: utf-8 -*-


%w[../xot ../rucy .]
  .map  {|s| File.expand_path "../../#{s}/lib", __FILE__}
  .each {|s| $:.unshift s if !$:.include?(s) && File.directory?(s)}

require 'test/unit'
require 'xot/test'
require 'beeps'

include Xot::Test


unless $BEEPS_NOAUTOINIT
  def Beeps.fin! () end
end
