require 'beeps/ext'


unless defined?($BEEPS_NOAUTOINIT) && $BEEPS_NOAUTOINIT
  Beeps.init!
  at_exit {Beeps.fin!}
end
