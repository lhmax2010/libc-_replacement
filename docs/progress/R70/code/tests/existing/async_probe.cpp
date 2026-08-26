#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <future>
#include <pthread.h>
#include <stdexcept>
#include <sys/syscall.h>
#include <unistd.h>

namespace {
std::atomic<unsigned long> worker_thread{0};
std::atomic<pid_t> worker_tid{0};

pid_t current_tid() { return static_cast<pid_t>(syscall(SYS_gettid)); }

void publish_worker() {
  worker_thread.store(static_cast<unsigned long>(pthread_self()), std::memory_order_release);
  worker_tid.store(current_tid(), std::memory_order_release);
}

int cancel_case() {
  auto future = std::async(std::launch::async, [] {
    publish_worker();
    for (;;)
      pthread_testcancel();
  });
  for (int i = 0; i < 500 && worker_thread.load(std::memory_order_acquire) == 0; ++i)
    usleep(10000);
  const int cancel_rc =
      pthread_cancel(static_cast<pthread_t>(worker_thread.load(std::memory_order_acquire)));
  std::fprintf(stderr, "MODE=cancel CANCEL_RC=%d WORKER_TID=%ld\n", cancel_rc,
               static_cast<long>(worker_tid.load(std::memory_order_acquire)));
  future.wait();
  std::fprintf(stderr, "MODE=cancel WAIT_RETURNED=1\n");
  std::fprintf(stderr, "MODE=cancel PTHREAD_JOIN=NOT_AVAILABLE_STD_ASYNC_OWNS_DETACHED_THREAD\n");
  try {
    future.get();
    std::fprintf(stderr, "MODE=cancel GET=value\n");
    return 0;
  } catch (const std::future_error& error) {
    std::fprintf(stderr, "MODE=cancel GET=future_error CODE=%d MESSAGE=%s\n",
                 error.code().value(), error.what());
    return error.code() == std::make_error_code(std::future_errc::broken_promise) ? 0 : 11;
  } catch (const std::exception& error) {
    std::fprintf(stderr, "MODE=cancel GET=std_exception MESSAGE=%s\n", error.what());
    return 12;
  } catch (...) {
    std::fprintf(stderr, "MODE=cancel GET=unknown_exception\n");
    return 13;
  }
}

int normal_case() {
  auto future = std::async(std::launch::async, [] { return 42; });
  const int value = future.get();
  std::fprintf(stderr, "MODE=normal VALUE=%d\n", value);
  return value == 42 ? 0 : 20;
}

int exception_case() {
  auto future = std::async(std::launch::async, []() -> int { throw std::runtime_error("async-real-error"); });
  try {
    (void)future.get();
    std::fprintf(stderr, "MODE=exception GET=value\n");
    return 30;
  } catch (const std::runtime_error& error) {
    std::fprintf(stderr, "MODE=exception GET=runtime_error MESSAGE=%s\n", error.what());
    return std::strcmp(error.what(), "async-real-error") == 0 ? 0 : 31;
  } catch (...) {
    std::fprintf(stderr, "MODE=exception GET=other_exception\n");
    return 32;
  }
}
} // namespace

int main(int argc, char** argv) {
  if (argc != 2)
    return 64;
  if (std::strcmp(argv[1], "cancel") == 0)
    return cancel_case();
  if (std::strcmp(argv[1], "normal") == 0)
    return normal_case();
  if (std::strcmp(argv[1], "exception") == 0)
    return exception_case();
  return 65;
}
