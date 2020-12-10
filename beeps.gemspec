# -*- mode: ruby; coding: utf-8 -*-


File.expand_path('../lib', __FILE__)
  .tap {|s| $:.unshift s if !$:.include?(s) && File.directory?(s)}

require 'beeps/module'


Gem::Specification.new do |s|
  glob = -> *patterns do
    patterns.map {|pat| Dir.glob(pat).to_a}.flatten
  end

  mod   = Beeps::Module
  name  = mod.name.downcase
  rdocs = glob.call *%w[README .doc/ext/**/*.cpp]

  s.name        = name
  s.summary     = 'Plays beep sound.'
  s.description = 'Synthesize and play beep sounds.'
  s.version     = mod.version

  s.authors  = %w[xordog]
  s.email    = 'xordog@gmail.com'
  s.homepage = "https://github.com/xord/beeps"

  s.platform              = Gem::Platform::RUBY
  s.required_ruby_version = '~> 2'

  s.add_runtime_dependency 'xot',  '~> 0.1.19'
  s.add_runtime_dependency 'rucy', '~> 0.1.18'

  s.files            = `git ls-files`.split $/
  s.executables      = s.files.grep(%r{^bin/}) {|f| File.basename f}
  s.test_files       = s.files.grep %r{^(test|spec|features)/}
  s.extra_rdoc_files = rdocs.to_a
  s.has_rdoc         = true

  s.extensions << 'Rakefile'
end
