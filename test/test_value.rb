require_relative 'helper'


class TestValue < Test::Unit::TestCase

  B = Beeps

  def val(...) = B::Value.new(...)

  def test_constant()
    assert_each_in_delta [5] * 3, get_samples(2, val(5))
  end

  def test_linear_interpolation()
    v = val 0
    v.insert 2, 1
    v.insert 1, 2
    assert_each_in_delta [0, 1, 2, 1.5, 1], get_samples(2, v, seconds: 2)
  end

  def test_insert()
    v = val 0
    v.insert 1, 1
    assert_equal [[0, 0], [1, 1]], v.to_a

    v.insert 2, 1
    assert_equal [[0, 0], [2, 1]], v.to_a

    assert_raise(ArgumentError) {v.insert 1, -1}
  end

  def test_clear()
    v = val 0
    assert_equal 1, v.to_a.size

    v.clear
    assert_equal 0, v.to_a.size
  end

  def test_each()
    v = val 0
    v.insert 1, 1
    v.insert 2, 2
    assert_equal [[0, 0], [1, 1], [2, 2]], v.to_a
  end

end# TestValue
