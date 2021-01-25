# -*- coding: utf-8 -*-


$BEEPS_NOAUTOINIT = true

require_relative 'helper'


class TestBeeps < Test::Unit::TestCase

  def test_init!()
    assert_raise(Rucy::NativeError) {Beeps.fin!}
    assert Beeps.init!
    assert_raise(Rucy::NativeError) {Beeps.init!}
    assert Beeps.fin!
  end

end# TestBeeps
