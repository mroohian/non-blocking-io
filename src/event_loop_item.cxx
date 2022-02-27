#include <unistd.h>
#include <iostream>
#include <sys/epoll.h>

#include "event_loop_item.hxx"

using namespace std;

event_loop_item::event_loop_item()
{
  this->fd = -1;
  this->closed = false;
  this->started = false;
}

bool event_loop_item::is_closed()
{
  return this->closed;
}

bool event_loop_item::is_started()
{
  return this->started;
}

int event_loop_item::get_key()
{
  return this->fd;
}

int event_loop_item::start(int epfd)
{
  struct epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.ptr = this;
  if (epoll_ctl(epfd, EPOLL_CTL_ADD, this->fd, &ev) == -1)
  {
    cout << "epoll_ctl(ADD) failed: errno=" << errno << endl;
    return -1;
  }

  this->started = true;

  return 0;
}

int event_loop_item::stop()
{
  if (this->is_closed())
  {
    cout << "event loop item is already closed" << endl;
    return -1;
  }

  if (close(this->fd) == -1)
  {
    cout << "failed to close fd:" << this->fd << " errno=" << errno << endl;
  }

  this->started = false;
  this->closed = true;

  return 0;
}

int safe_delete_item(event_loop_item *item)
{
  int ret = 0;

  if (!item->is_closed())
  {
    if (item->stop() == -1)
    {
      cout << "failed to stop the event loop item:" << endl;
      ret = -1;
    }
  }

  delete item;

  return ret;
}