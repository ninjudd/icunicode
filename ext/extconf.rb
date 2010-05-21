# This file was derived from Evan's Weaver memcached library: http://github.com/fauna/memcached
# See the ext/LICENSE_AFL3 file.

require 'mkmf'
require 'rbconfig'

HERE   = File.expand_path(File.dirname(__FILE__))
BUNDLE = Dir.glob("icu4c-*.tgz").first

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
      puts(cmd = "tar xzf #{BUNDLE} 2>&1")
      raise "'#{cmd}' failed" unless system(cmd)

      Dir.chdir("icu/source") do
        puts(cmd = "./configure --prefix=#{HERE} --disable-shared --enable-static 2>&1")
        raise "'#{cmd}' failed" unless system(cmd)

        puts(cmd = "make || true 2>&1")
        raise "'#{cmd}' failed" unless system(cmd)

        puts(cmd = "make install || true 2>&1")
        raise "'#{cmd}' failed" unless system(cmd)
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
