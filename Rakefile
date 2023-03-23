# -*- mode: ruby; coding: utf-8 -*-


%w[../xot ../rucy .]
  .map  {|s| File.expand_path "#{s}/lib", __dir__}
  .each {|s| $:.unshift s if !$:.include?(s) && File.directory?(s)}

require 'rucy/rake'

require 'xot/extension'
require 'rucy/extension'
require 'beeps/extension'


EXTENSIONS  = [Xot, Rucy, Beeps]
TESTS_ALONE = ['test/test_beeps_init.rb']

use_external_library 'https://github.com/thestk/stk',
  tag:      '4.6.2',
  incdirs:  'include',
  srcdirs:  'src',
  excludes: %w[stk/src/include Tcp Udp Socket Thread Mutex InetWv /Rt]

use_external_library 'https://github.com/avaneev/r8brain-free-src.git',
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

default_tasks :ext
build_native_library
build_ruby_extension
test_ruby_extension
generate_documents
build_ruby_gem
