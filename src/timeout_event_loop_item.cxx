#include <unistd.h>
#include <sys/timerfd.h>
#include <iostream>

#include "callback.hxx"
#include "timeout_event_loop_item.hxx"

using namespace std;

timeout_event_loop_item::timeout_event_loop_item(int msec, CALLBACK(cb)) : event_loop_item()
{
  this->msec = msec;
  this->cb = cb;
}

int timeout_event_loop_item::start(int epfd)
{
  this->fd = timerfd_create(CLOCK_MONOTONIC, 0);
  if (this->fd == -1)
  {
    cout << "timerfd_create() failed: errno=" << errno << endl;
    this->closed = true;
    return -1;
  }

  struct itimerspec ts;
  ts.it_interval.tv_sec = 0;
  ts.it_interval.tv_nsec = 0;
  ts.it_value.tv_sec = this->msec / 1000;
  ts.it_value.tv_nsec = (this->msec % 1000) * 1000000;

  if (timerfd_settime(this->fd, 0, &ts, NULL) < 0)
  {
    cout << "timerfd_settime() failed: errno=" << errno << endl;
    close(this->fd);
    this->closed = true;
    return -1;
  }

  return event_loop_item::start(epfd);
}

void timeout_event_loop_item::execute()
{
  uint64_t res;
  read(this->fd, &res, sizeof(res));

  this->cb();

  this->stop();
}