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

    case
    when osx?
      headers    << 'OpenAL/al.h' << 'OpenAL/alc.h'
      frameworks << 'OpenAL'      << 'AVFoundation'
    when win32?
      headers    << 'AL/al.h'     << 'AL/alc.h'
      libs       << 'openal' << 'ole32' << 'mf' << 'mfplat' << 'mfreadwrite' << 'mfuuid'
    when linux?
      headers    << 'AL/al.h'     << 'AL/alc.h'
      libs       << 'openal'
    end

    $LDFLAGS << ' -Wl,--out-implib=beeps_ext.dll.a' if mingw? || cygwin?
  end

  create_makefile 'beeps_ext'
end
