# mruby-eventfd   [![Build Status](https://travis-ci.org/matsumotory/mruby-eventfd.png?branch=master)](https://travis-ci.org/matsumotory/mruby-eventfd)
Eventfd class
## install by mrbgems
- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

  # ... (snip) ...

  conf.gem :github => 'matsumoto-r/mruby-eventfd'
  # for below example
  conf.gem :github => "iij/mruby-process"
  conf.gem :github => "iij/mruby-io"
  conf.gem :github => "iij/mruby-dir"

end
```
## example
```ruby
e = Eventfd.new 0, 0

p e.fd

pid = Process.fork() do
  p "child fork... sleep 5"
  sleep 5
  e.event_write 999
end

p "event read..."

e.event_read do |ret|
  p ret
  p e.close
end
```
## example run
```
$ ./bin/mruby test.rb
4
"event read..."
"child fork... sleep 5"
999
0
```

## License
under the MIT License:
- see LICENSE file
