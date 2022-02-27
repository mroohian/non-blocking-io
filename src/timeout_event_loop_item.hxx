#pragma once

#include "callback.hxx"
#include "event_loop_item.hxx"

class timeout_event_loop_item : public event_loop_item
{
private:
  int msec;

public:
  timeout_event_loop_item(int msec, CALLBACK(cb));
  int start(int epfd);
  void execute();
};