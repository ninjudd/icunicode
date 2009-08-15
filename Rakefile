require 'rake'
require 'rake/testtask'
require 'rake/rdoctask'

begin
  require 'jeweler'
  Jeweler::Tasks.new do |s|
    s.name = "icunicode"
    s.summary = %Q{Unicode Transliteration and Collation in Ruby.}
    s.email = "code@justinbalthrop.com"
    s.homepage = "http://github.com/ninjudd/unicode_collation"
    s.description = "ICU Unicode Transliteration and Collation in Ruby."
    s.authors = ["Justin Balthrop"]
    s.files = ["README.rdoc", "VERSION.yml", "ext/icunicode.c", "ext/extconf.rb", "test/test_helper.rb", "test/icunicode_test.rb"]
    s.extensions = ["ext/extconf.rb"]
    s.require_paths = ["ext"]
  end
rescue LoadError
  puts "Jeweler not available. Install it with: sudo gem install technicalpickles-jeweler -s http://gems.github.com"
end

Rake::TestTask.new do |t|
  t.libs << "ext"
  t.pattern = 'test/**/*_test.rb'
  t.verbose = false
end

Rake::RDocTask.new do |rdoc|
  rdoc.rdoc_dir = 'rdoc'
  rdoc.title    = 'tuple'
  rdoc.options << '--line-numbers' << '--inline-source'
  rdoc.rdoc_files.include('README*')
  rdoc.rdoc_files.include('lib/**/*.rb')
end

begin
  require 'rcov/rcovtask'
  Rcov::RcovTask.new do |t|
    t.libs << 'test'
    t.test_files = FileList['test/**/*_test.rb']
    t.verbose = true
  end
rescue LoadError
end

desc "Clean"
task :clean do
  include FileUtils
  Dir.chdir('ext') do
    rm Dir.glob('*.{o,bundle,so}')
    rm_f 'Makefile'
    rm_f 'mkmf.log'
  end
  rm_rf 'pkg'
end

task :default => :test
