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
