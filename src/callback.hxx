#pragma once

#include <functional>

#define CALLBACK(name) std::function<void(void)> name
