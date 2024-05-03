require 'beeps/ext'


p [38, Time.now.to_f]
unless defined?($BEEPS_NOAUTOINIT) && $BEEPS_NOAUTOINIT
  p [30, Time.now.to_f]
  Beeps.init!
  p [31, Time.now.to_f]
  at_exit {Beeps.fin!}
  p [32, Time.now.to_f]
end
p [39, Time.now.to_f]
