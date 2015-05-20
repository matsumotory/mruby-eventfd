MRuby::Build.new do |conf|
  toolchain :gcc
  conf.gembox 'default'
  conf.gem '../mruby-eventfd'
  conf.gem :github => 'iij/mruby-process'
  conf.gem :github => 'matsumoto-r/mruby-sleep'
end
