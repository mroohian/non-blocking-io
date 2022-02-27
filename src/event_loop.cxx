#include <unistd.h>
#include <sys/epoll.h>
#include <iostream>

#include "event_loop.hxx"
#include "event_loop_item.hxx"

using namespace std;

#define MAX_EVENTS 10

event_loop::event_loop()
{
  this->closed = false;
  this->epfd = epoll_create1(0);
  if (this->epfd == -1)
  {
    this->closed = true;
    cout << "epoll_create1() failed: errno=" << errno << endl;
  }
}

bool event_loop::is_closed()
{
  return this->closed;
}

int event_loop::add_item(event_loop_item *item)
{
  if (this->is_closed())
  {
    cout << "event loop is already closed" << endl;
    return -1;
  }

  if (item->is_closed())
  {
    cout << "cannot add an item that is already closed" << endl;
    return -1;
  }

  if (item->is_started())
  {
    cout << "item is already started" << endl;
    return -1;
  }

  if (item->start(this->epfd) == -1)
  {
    cout << "failed to start event loop item" << endl;
    return -1;
  }

  int item_key = item->get_key();
  this->items[item_key] = item;

  return 0;
}

int event_loop::remove_item(event_loop_item *item)
{
  int item_key = item->get_key();

  safe_delete_item(item);

  this->items.erase(item_key);

  return 0;
}

int event_loop::schedule(event_loop_item *item)
{
  if (this->add_item(item) == -1)
  {
    cout << "loop->add_item() failed" << endl;

    safe_delete_item(item);

    return -1;
  }

  return 0;
}

int event_loop::stop()
{
  if (this->is_closed())
  {
    cout << "event loop is already closed" << endl;
    return 0;
  }

  this->closed = true;

  if (close(this->epfd) == -1)
  {
    cout << "failed to close epollfd: errno=" << errno << endl;
    return -1;
  }

  for (auto x : this->items)
  {
    event_loop_item *item = x.second;

    safe_delete_item(item);
  }

  this->items.clear();

  return 0;
}

int event_loop::run_until_complete()
{
  if (this->is_closed())
  {
    cout << "event loop is already closed" << endl;
    return -1;
  }

  struct epoll_event events[MAX_EVENTS];

  for (;;)
  {
    if (this->is_closed() || this->items.size() == 0)
    {
      break;
    }

    int nfds = epoll_wait(this->epfd, events, MAX_EVENTS, -1);
    if (nfds == -1)
    {
      cout << "epoll_wait() failed: errno=" << errno << endl;
      this->stop();
      return -1;
    }

    for (int n = 0; n < nfds; ++n)
    {
      event_loop_item *item = (event_loop_item *)events[n].data.ptr;
      if (!item->is_started())
      {
        cout << "notified about an item that is not started" << endl;
      }

      item->execute();
      if (item->is_closed())
      {
        this->remove_item(item);
      }
    }
  }

  if (this->stop() == -1)
  {
    cout << "failed to close event loop" << endl;
    return -1;
  }

  return 0;
}

event_loop *loop_create()
{
  event_loop *loop = new event_loop();

  if (loop->is_closed())
  {
    cout << "failed to create event_loop" << endl;
    delete loop;
    return NULL;
  }

  return loop;
}

int loop_close(event_loop *loop)
{
  if (!loop->is_closed())
  {
    loop->stop();
  }

  delete loop;

  return 0;
}
