#pragma once

#include <unordered_map>

class event_loop_item;

class event_loop
{
private:
  bool closed;
  int epfd;
  std::unordered_map<int, event_loop_item *> items;

private:
  int add_item(event_loop_item *item);
  int remove_item(event_loop_item *item);

public:
  event_loop();
  bool is_closed();
  int schedule(event_loop_item *item);
  int stop();
  int run_until_complete();
};

event_loop *loop_create();
int loop_close(event_loop *loop);