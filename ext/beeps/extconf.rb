# -*- coding: utf-8 -*-


%w[../xot ../rucy .]
  .map  {|s| File.expand_path "../../../#{s}/lib", __FILE__}
  .each {|s| $:.unshift s if !$:.include?(s) && File.directory?(s)}

require 'mkmf'
require 'xot/extconf'
require 'xot/module'
require 'rucy/module'
require 'beeps/module'


Xot::ExtConf.new Xot, Rucy, Beeps do
  setup do
    headers    << 'OpenAL/al.h' << 'OpenAL/alc.h'
    local_libs << 'rucy'
    frameworks << 'OpenAL' if osx?
    $LDFLAGS   << ' -Wl,--out-implib=native.dll.a' if cygwin?
  end

  dir_config 'boost'
  create_makefile 'beeps/native'
end
