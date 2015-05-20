assert "Eventfd#new" do
  e = Eventfd.new 0,0
  assert_true(e.fd > 0)
  e.close
end

assert "Eventfd#cloase" do
  e = Eventfd.new 0,0
  assert_equal 0, e.close
end

assert "Eventfd#event_rad,event_write" do
  e = Eventfd.new 0, 0
  Process.fork() do
    sleep 3
    e.event_write 999
  end
  e.event_read do |ret|
    assert_equal 999, ret
    e.close
  end
end
