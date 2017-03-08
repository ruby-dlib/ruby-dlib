# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'dlib/version'

Gem::Specification.new do |spec|
  spec.name          = "dlib"
  spec.version       = Dlib::VERSION
  spec.authors       = ["Kenta Murata", "Satoshi Namai"]
  spec.email         = ["mrkn@mrkn.jp", "s.namai.09@gmail.com"]
  spec.summary       = %q{Ruby bindings of dlib C++ library.}
  spec.description   = %q{Ruby bindings of dlib C++ library.}
  spec.homepage      = "https://github.com/ruby-dlib/ruby-dlib"
  spec.license       = "MIT"

  spec.files         = `git ls-files -z`.split("\x0")
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]
  spec.extensions    << "ext/dlib/extconf.rb"

  spec.add_development_dependency "bundler", "~> 1.7"
  spec.add_development_dependency "rake", "~> 10.0"
end
