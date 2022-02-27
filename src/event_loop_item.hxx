#pragma once

#include "callback.hxx"

class event_loop_item
{
protected:
  int fd;
  bool closed;
  bool started;
  CALLBACK(cb);

public:
  event_loop_item();
  bool is_closed();
  bool is_started();
  int get_key();
  virtual int start(int epfd);
  virtual void execute() = 0;
  int stop();
};

int safe_delete_item(event_loop_item *item);
