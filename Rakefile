# -*- mode: ruby -*-

%w[../xot ../rucy .]
  .map  {|s| File.expand_path "#{s}/lib", __dir__}
  .each {|s| $:.unshift s if !$:.include?(s) && File.directory?(s)}

require 'rucy/rake'

require 'xot/extension'
require 'rucy/extension'
require 'beeps/extension'


EXTENSIONS  = [Xot, Rucy, Beeps]
TESTS_ALONE = ['test/test_beeps_init.rb']

install_packages(
  mingw: %w[MINGW_PACKAGE_PREFIX-openal],
  apt:   %w[libopenal-dev])

use_external_library 'https://github.com/thestk/stk',
  tag:      '5.0.1',
  incdirs:  'include',
  srcdirs:  'src',
  excludes: %w[stk/src/include Tcp Udp Socket Thread Mutex InetWv /Rt]

use_external_library 'https://github.com/adamstark/AudioFile',
  tag:      '1.1.1',
  srcdirs:  'NOSRC',
  excludes: %w[examples/ tests/]

use_external_library 'https://github.com/avaneev/r8brain-free-src',
  tag: 'version-6.2',
  excludes: %w[DLL/ bench/ other/ pffft_double example.cpp],
  &proc {
    filter_file('r8bconf.h') do |conf|
      <<~EOS + conf
        #ifndef R8B_PFFFT
        #define R8B_PFFFT 1
        #endif
      EOS
    end
  }

use_external_library 'https://github.com/Signalsmith-Audio/signalsmith-stretch',
  commit:  'fddcdb628f326ef187b7e5694f285f35a528aba3',
  srcdirs: 'NOSRC'

default_tasks :ext
use_bundler
build_native_library
build_ruby_extension
test_ruby_extension unless github_actions? && win32?
generate_documents
build_ruby_gem
