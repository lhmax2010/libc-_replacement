#include "boundary_error.hpp"

#include <atomic>
#include <condition_variable>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <stdexcept>
#include <string>

namespace {
std::atomic<int> old_guard_count{0};
std::atomic<int> wait_ready{0};
std::mutex wait_mutex;
std::condition_variable wait_cv;

struct OldGuard {
  ~OldGuard() { old_guard_count.fetch_add(1, std::memory_order_relaxed); }
};
}  // namespace

extern "C" __attribute__((visibility("default"))) int
old_normal(const char* input, char* output, unsigned output_size) {
  OldGuard guard;
  std::string value = "old:";
  value += input;
  if (value.size() + 1 > output_size)
    return -1;
  std::memcpy(output, value.c_str(), value.size() + 1);
  return static_cast<int>(value.size());
}

extern "C" __attribute__((visibility("default"))) void old_throw() {
  OldGuard guard;
  throw BoundaryError{77};
}

extern "C" __attribute__((visibility("default"))) void old_cancel_wait() {
  OldGuard guard;
  std::unique_lock<std::mutex> lock(wait_mutex);
  wait_ready.store(1, std::memory_order_release);
  wait_cv.wait(lock, [] { return false; });
}

extern "C" __attribute__((visibility("default"))) int old_wait_ready() {
  return wait_ready.load(std::memory_order_acquire);
}

extern "C" __attribute__((visibility("default"))) int old_guard_value() {
  return old_guard_count.load(std::memory_order_relaxed);
}
