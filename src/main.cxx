#include <iostream>

#include "event_loop.hxx"
#include "event_loop_events.hxx"

using namespace std;

int main()
{
  int result;

  event_loop *loop = loop_create();
  if (loop == NULL)
  {
    cout << "loop_create() failed" << endl;
    return EXIT_FAILURE;
  }

  result = set_timeout(loop, 2000, []() {
    cout << "first timer triggered" << endl;
  });
  if (result == -1)
  {
    cout << "loop_sleep() failed" << endl;
    loop_close(loop);
    return EXIT_FAILURE;
  }

  int n = 0;
  result = set_timeout(loop, 4000, [&]() {
    cout << "second timer triggered. n:" << (n++) << endl;

    set_timeout(loop, 2000, [&]() {
      cout << "third timer triggered. n:" << (n++) << endl;

      set_timeout(loop, 2000, [&]() {
        cout << "fourth timer triggered. n:" << (n++) << endl;
      });
    });
  });
  if (result == -1)
  {
    cout << "loop_sleep() failed" << endl;
    loop_close(loop);
    return EXIT_FAILURE;
  }

  result = loop->run_until_complete();
  if (result == -1)
  {
    cout << "loop_run_until_complete() failed" << endl;
    loop_close(loop);
    return EXIT_FAILURE;
  }

  result = loop_close(loop);
  if (result == -1)
  {
    cout << "loop_close() failed" << endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
