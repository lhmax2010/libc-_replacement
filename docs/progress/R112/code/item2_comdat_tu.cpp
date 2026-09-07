#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <mutex>
#include <system_error>

#ifndef ENTRY_NAME
#error ENTRY_NAME is required
#endif

extern "C" int ENTRY_NAME() {
  std::condition_variable cv;
  std::mutex mutex;
  std::unique_lock<std::mutex> lock(mutex, std::defer_lock);
  try {
    cv.wait_until(lock, std::chrono::steady_clock::now() + std::chrono::seconds(1));
    std::puts("event=unexpected_return");
    return 67;
  } catch (const std::system_error& error) {
    std::printf("event=caught_system_error code=%d message=%s\n", error.code().value(), error.what());
    return 42;
  } catch (...) {
    std::puts("event=other_exception");
    return 43;
  }
}
