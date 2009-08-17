require File.dirname(__FILE__) + '/test_helper'

class UnicodeCollationTest < Test::Unit::TestCase
  should "sort using unicode collation" do
    assert_equal ["cafe", "café", "cafes"], ["cafe", "cafes", "café"].sort_by {|s| s.unicode_sort_key}
    assert_equal ["role", "Role", "rôle"],  ["rôle", "role", "Role"].sort_by {|s| s.unicode_sort_key}
    assert_equal ["cote", "coté", "côte", "côté"], ["côté", "coté", "cote", "côte"].sort_by {|s| s.unicode_sort_key}
  end

  should "transliterate" do
    assert_equal "ムクドナルデ'ス",         "mcdonald's".transliterate('Katakana')
    assert_equal "さむらい",               "samurai".transliterate('Hiragana')
    assert_equal "θε γρεατ γρεεκ",         "the great greek".transliterate('Greek')
    assert_equal "фром руссиа уитх лове", "from russia with love".transliterate('Cyrillic')
  end
end
