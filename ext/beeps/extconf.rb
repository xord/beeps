# -*- coding: utf-8 -*-


%w[../xot ../rucy .]
  .map  {|s| File.expand_path "../../#{s}/lib", __dir__}
  .each {|s| $:.unshift s if !$:.include?(s) && File.directory?(s)}

require 'mkmf'
require 'xot/extconf'
require 'xot/extension'
require 'rucy/extension'
require 'beeps/extension'


Xot::ExtConf.new Xot, Rucy, Beeps do
  setup do
    headers    << 'ruby.h' << 'OpenAL/al.h' << 'OpenAL/alc.h'
    local_libs << 'rucy'
    frameworks << 'OpenAL' if osx?
    $LDFLAGS   << ' -Wl,--out-implib=native.dll.a' if cygwin?
  end

  create_makefile 'beeps/native'
end
