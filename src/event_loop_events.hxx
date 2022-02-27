#pragma once

#include "callback.hxx"
#include "event_loop.hxx"

int set_timeout(event_loop *loop, int msec, CALLBACK(cb));
