# -*- coding: utf-8 -*-


require 'beeps/ext'


unless $BEEPS_NOAUTOINIT
  Beeps.init!
  at_exit {Beeps.fin!}
end
