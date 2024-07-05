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
    headers << 'ruby.h'

    if win32?
      headers    << 'AL/al.h'     << 'AL/alc.h'
      libs       << 'openal' << 'ole32' << 'mf' << 'mfplat' << 'mfreadwrite' << 'mfuuid'
    elsif osx?
      headers    << 'OpenAL/al.h' << 'OpenAL/alc.h'
      frameworks << 'OpenAL'      << 'AVFoundation'
    end

    $LDFLAGS << ' -Wl,--out-implib=native.dll.a' if mingw? || cygwin?
  end

  create_makefile 'beeps/native'
end
