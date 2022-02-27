#include <unistd.h>
#include <sys/epoll.h>
#include <iostream>

#include "callback.hxx"
#include "event_loop.hxx"
#include "timeout_event_loop_item.hxx"

using namespace std;

int set_timeout(event_loop *loop, int msec, CALLBACK(cb)) {
  return loop->schedule(new timeout_event_loop_item(msec, cb));
}
