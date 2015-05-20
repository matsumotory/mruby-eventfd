MRuby::Gem::Specification.new('mruby-eventfd') do |spec|
  spec.license = 'MIT'
  spec.authors = 'MATSUMOTO Ryosuke'
  # for test dependency
  spec.add_dependency 'mruby-process'
  spec.add_dependency 'mruby-sleep'
end
