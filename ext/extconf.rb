# This file was derived from Evan's Weaver memcached library: http://github.com/fauna/memcached
# See the ext/LICENSE_AFL3 file.

require 'mkmf'
require 'rbconfig'

HERE   = File.expand_path(File.dirname(__FILE__))
BUNDLE = "icu4c-4_4_1-src.tgz"
REPO   = "http://download.icu-project.org/files/icu4c/4.4.1"
GET    = `which curl`.empty? ? "wget -O" : "curl -L -o"

def sh(cmd)
  puts(cmd)
  raise "'#{cmd}' failed" unless system(cmd)
end

if !ENV["EXTERNAL_LIB"]
  $includes    = " -I#{HERE}/include"
  $libraries   = " -L#{HERE}/lib"
  $CFLAGS      = "#{$includes} #{$libraries} #{$CFLAGS}"
  $LDFLAGS     = "#{$libraries} #{$LDFLAGS}"
  $LIBPATH     = ["#{HERE}/lib"]
  $DEFLIBPATH  = []

  Dir.chdir(HERE) do
    if File.exist?("lib")
      puts "ICU already built; run 'rake clean' first if you need to rebuild."
    else
      puts "Building ICU."
      sh("#{GET} #{BUNDLE} #{REPO}/#{BUNDLE}")
      sh("tar xzf #{BUNDLE} 2>&1")

      Dir.chdir("icu/source") do
        sh("./configure --prefix=#{HERE} --disable-shared --enable-static 2>&1")
        sh("make || true 2>&1")
        sh("make install || true 2>&1")
      end

      system("rm -rf icu") unless ENV['DEBUG'] or ENV['DEV']
    end
  end

  # Absolutely prevent the linker from picking up any other libmemcached
  Dir.chdir("#{HERE}/lib") do
    system('cp -f libicui18n.a libicui18n_gem.a')
    system('cp -f libicuio.a  libicuio_gem.a')
  end

  $LIBS << " -licui18n_gem"
  $LIBS << " -licuio_gem"
end

create_makefile('icunicode')
