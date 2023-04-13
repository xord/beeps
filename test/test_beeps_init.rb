# -*- coding: utf-8 -*-


$BEEPS_NOAUTOINIT = true

require_relative 'helper'


class TestBeepsInit < Test::Unit::TestCase

  def test_init!()
    assert_raise(Beeps::BeepsError) {Beeps.fin!}
    assert Beeps.init!
    assert_raise(Beeps::BeepsError) {Beeps.init!}
    assert Beeps.fin!
  end

end# TestBeepsInit
