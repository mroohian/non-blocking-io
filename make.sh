rm -rf build && \
  mkdir build && \
  cd build && \
  cmake .. && \
  cmake --build . && \
  cp non_blocking_io ..
