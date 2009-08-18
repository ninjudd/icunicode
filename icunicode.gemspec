Gem::Specification.new do |s|
  s.name = %q{icunicode}
  s.version = "0.0.2"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Justin Balthrop"]
  s.date = %q{2009-08-17}
  s.description = %q{ICU Unicode Transliteration and Collation in Ruby.}
  s.email = %q{code@justinbalthrop.com}
  s.extensions = ["ext/extconf.rb"]
  s.files = ["README.rdoc", "VERSION.yml", "ext/icunicode.c", "ext/extconf.rb", "test/test_helper.rb", "test/icunicode_test.rb"]
  s.has_rdoc = true
  s.homepage = %q{http://github.com/ninjudd/unicode_collation}
  s.rdoc_options = ["--inline-source", "--charset=UTF-8"]
  s.require_paths = ["ext"]
  s.rubygems_version = %q{1.2.0}
  s.summary = %q{Unicode Transliteration and Collation in Ruby.}

  if s.respond_to? :specification_version then
    current_version = Gem::Specification::CURRENT_SPECIFICATION_VERSION
    s.specification_version = 2

    if current_version >= 3 then
    else
    end
  else
  end
end
