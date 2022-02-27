cmake_minimum_required(VERSION 3.10)

project(non_blocking_io VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(non_blocking_io src/main.cxx)

target_sources(non_blocking_io PRIVATE
  src/event_loop.cxx
  src/event_loop_item.cxx
  src/timeout_event_loop_item.cxx
  src/event_loop_events.cxx
)

target_include_directories(non_blocking_io PUBLIC include)
target_include_directories(non_blocking_io PUBLIC "${PROJECT_BINARY_DIR}/include")
