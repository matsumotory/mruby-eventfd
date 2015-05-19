/*
** mrb_eventfd.c - Eventfd class
**
** Copyright (c) MATSUMOTO Ryosuke 2015
**
** See Copyright Notice in LICENSE
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "mruby.h"
#include "mruby/data.h"
#include "mrb_eventfd.h"
#include <sys/eventfd.h>

#define DONE mrb_gc_arena_restore(mrb, 0);
#define EVENTFD_WRTE_NUMBER 7

typedef struct {
  mrb_int intval;
  mrb_int flags;
  int fd;
  uint64_t buf;
} mrb_eventfd_data;

static const struct mrb_data_type mrb_eventfd_data_type = {
  "mrb_eventfd_data", mrb_free,
};

static mrb_value mrb_eventfd_init(mrb_state *mrb, mrb_value self)
{
  mrb_eventfd_data *data;
  mrb_int intval, flags;

  data = (mrb_eventfd_data *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &mrb_eventfd_data_type;
  DATA_PTR(self) = NULL;

  mrb_get_args(mrb, "ii", &intval, &flags);
  data = (mrb_eventfd_data *)mrb_malloc(mrb, sizeof(mrb_eventfd_data));
  data->intval = intval;
  data->flags = flags;
  data->fd = eventfd(intval, flags);
  data->buf = 0;
  DATA_PTR(self) = data;

  return self;
}

static mrb_value mrb_eventfd_fd(mrb_state *mrb, mrb_value self)
{
  mrb_eventfd_data *data = DATA_PTR(self);
  return mrb_fixnum_value(data->fd);
}

static mrb_value mrb_eventfd_read(mrb_state *mrb, mrb_value self)
{
  mrb_eventfd_data *data = DATA_PTR(self);
  uint64_t buf;
  mrb_value b, arg;

  read(data->fd, &buf, sizeof(uint64_t));
  data->buf = buf;

  mrb_get_args(mrb, "&", &b);
  arg = mrb_fixnum_value(buf);

  return mrb_yield_argv(mrb, b, 1, &arg);
}

static mrb_value mrb_eventfd_write(mrb_state *mrb, mrb_value self)
{
  mrb_eventfd_data *data = DATA_PTR(self);
  uint64_t buf = EVENTFD_WRTE_NUMBER;
  mrb_get_args(mrb, "|i", &buf);
  return mrb_fixnum_value(write(data->fd, &buf, sizeof(uint64_t)));
}

static mrb_value mrb_eventfd_close(mrb_state *mrb, mrb_value self)
{
  mrb_eventfd_data *data = DATA_PTR(self);
  int ret = close(data->fd);
  return mrb_fixnum_value(ret);
}

void mrb_mruby_eventfd_gem_init(mrb_state *mrb)
{
    struct RClass *eventfd;
    eventfd = mrb_define_class(mrb, "Eventfd", mrb->object_class);
    mrb_define_method(mrb, eventfd, "initialize", mrb_eventfd_init, MRB_ARGS_REQ(2));
    mrb_define_method(mrb, eventfd, "fd", mrb_eventfd_fd, MRB_ARGS_NONE());
    mrb_define_method(mrb, eventfd, "event_read", mrb_eventfd_read, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, eventfd, "event_write", mrb_eventfd_write, MRB_ARGS_OPT(1));
    mrb_define_method(mrb, eventfd, "close", mrb_eventfd_close, MRB_ARGS_NONE());
    DONE;
}

void mrb_mruby_eventfd_gem_final(mrb_state *mrb)
{
}

