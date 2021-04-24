require 'test_helper'

class ContinueGetTest < BaseTest
  def test_continue_get_multi
    cache.set "foo", "bar"
    cache.set "another", "baz"
    cache.begin_get_multi ["foo", "another"]
    
    while true do
      result_hash, readers, writers = cache.continue_get_multi
      break if readers.length == 0 && writers.length == 0
      IO.select(readers, [])
    end

    assert_equal({ "foo" => "bar", "another" => "baz" }, result_hash)
  end
end
