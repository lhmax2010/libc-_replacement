#include <atomic>
#include <chrono>
#include <cstring>
#include <future>
#include <iostream>
#include <pthread.h>
#include <stdexcept>
#include <thread>
#include <vector>
#include <unistd.h>

namespace {

std::atomic<int> window_entered{0};
std::atomic<int> allow_return{0};
pthread_t worker_thread{};

struct WindowResult {
  int value = 61;
  bool hold_worker = true;

  WindowResult() = default;
  WindowResult(const WindowResult&) = delete;
  WindowResult& operator=(const WindowResult&) = delete;
  WindowResult(WindowResult&& other) noexcept : value(other.value), hold_worker(false) {}

  ~WindowResult() noexcept(false) {
    if (!hold_worker)
      return;
    window_entered.store(1, std::memory_order_release);
    for (;;)
      asm volatile("" ::: "memory");
  }
};

WindowResult make_window_result() {
  worker_thread = pthread_self();
  int old_type = 0;
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old_type);
  return WindowResult{};
}

bool is_broken_promise(const std::future_error& error) {
  return error.code() == std::make_error_code(std::future_errc::broken_promise);
}

int cancel_in_window() {
  auto future = std::async(std::launch::async, make_window_result);
  while (!window_entered.load(std::memory_order_acquire))
    usleep(1000);
  const int cancel_rc = pthread_cancel(worker_thread);
  try {
    (void)future.get();
    std::cout << "M2_RESULT=VALUE CANCEL_RC=" << cancel_rc << '\n' << std::flush;
    return 2;
  } catch (const std::future_error& error) {
    const bool broken = is_broken_promise(error);
    std::cout << "M2_RESULT=FUTURE_ERROR BROKEN_PROMISE=" << broken << " CANCEL_RC=" << cancel_rc
              << " WHAT=" << error.what() << '\n'
              << std::flush;
    return cancel_rc == 0 && broken ? 0 : 3;
  }
}

int multiple_waiters() {
  auto future = std::async(std::launch::async, make_window_result).share();
  while (!window_entered.load(std::memory_order_acquire))
    usleep(1000);

  constexpr int waiter_count = 4;
  std::atomic<int> started{0};
  std::atomic<int> done{0};
  std::atomic<int> broken{0};
  std::vector<std::thread> waiters;
  for (int i = 0; i != waiter_count; ++i) {
    waiters.emplace_back([&] {
      started.fetch_add(1, std::memory_order_release);
      try {
        (void)future.get();
      } catch (const std::future_error& error) {
        if (is_broken_promise(error))
          broken.fetch_add(1, std::memory_order_relaxed);
      }
      done.fetch_add(1, std::memory_order_release);
    });
  }
  while (started.load(std::memory_order_acquire) != waiter_count)
    usleep(1000);
  const int cancel_rc = pthread_cancel(worker_thread);
  for (int i = 0; i != 5000 && done.load(std::memory_order_acquire) != waiter_count; ++i)
    usleep(1000);
  if (done.load(std::memory_order_acquire) != waiter_count) {
    std::cout << "MULTI_WAITERS=TIMEOUT DONE=" << done.load() << " CANCEL_RC=" << cancel_rc << '\n' << std::flush;
    _Exit(4);
  }
  for (auto& waiter : waiters)
    waiter.join();
  std::cout << "MULTI_WAITERS=DONE COUNT=" << done.load() << " BROKEN=" << broken.load()
            << " CANCEL_RC=" << cancel_rc << '\n'
            << std::flush;
  return cancel_rc == 0 && broken.load() == waiter_count ? 0 : 5;
}

int wait_for_path() {
  auto future = std::async(std::launch::async, [] {
    while (!allow_return.load(std::memory_order_acquire))
      usleep(1000);
    return make_window_result();
  });
  const auto first = future.wait_for(std::chrono::milliseconds(20));
  allow_return.store(1, std::memory_order_release);
  while (!window_entered.load(std::memory_order_acquire))
    usleep(1000);
  const int cancel_rc = pthread_cancel(worker_thread);
  const auto second = future.wait_for(std::chrono::seconds(2));
  bool broken = false;
  try {
    (void)future.get();
  } catch (const std::future_error& error) {
    broken = is_broken_promise(error);
  }
  std::cout << "WAIT_FOR_FIRST_TIMEOUT=" << (first == std::future_status::timeout)
            << " SECOND_READY=" << (second == std::future_status::ready) << " BROKEN=" << broken
            << " CANCEL_RC=" << cancel_rc << '\n'
            << std::flush;
  return first == std::future_status::timeout && second == std::future_status::ready && broken && cancel_rc == 0 ? 0
                                                                                                                 : 6;
}

int void_result() {
  auto future = std::async(std::launch::async, [] {
    worker_thread = pthread_self();
    int old_type = 0;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old_type);
    window_entered.store(1, std::memory_order_release);
    for (;;)
      asm volatile("" ::: "memory");
  });
  while (!window_entered.load(std::memory_order_acquire))
    usleep(1000);
  const int cancel_rc = pthread_cancel(worker_thread);
  bool broken = false;
  try {
    future.get();
  } catch (const std::future_error& error) {
    broken = is_broken_promise(error);
  }
  std::cout << "VOID_BROKEN=" << broken << " CANCEL_RC=" << cancel_rc << '\n' << std::flush;
  return broken && cancel_rc == 0 ? 0 : 7;
}

int ordinary_exception() {
  auto future = std::async(std::launch::async, []() -> int { throw std::runtime_error("r61 ordinary"); });
  try {
    (void)future.get();
  } catch (const std::runtime_error& error) {
    const bool match = std::strcmp(error.what(), "r61 ordinary") == 0;
    std::cout << "ORDINARY_EXCEPTION=RUNTIME_ERROR MESSAGE_MATCH=" << match << '\n' << std::flush;
    return match ? 0 : 8;
  } catch (...) {
    std::cout << "ORDINARY_EXCEPTION=WRONG_TYPE\n" << std::flush;
    return 9;
  }
  std::cout << "ORDINARY_EXCEPTION=NOT_THROWN\n" << std::flush;
  return 10;
}

} // namespace

int main(int argc, char** argv) {
  if (argc != 2)
    return 64;
  if (std::strcmp(argv[1], "m2") == 0)
    return cancel_in_window();
  if (std::strcmp(argv[1], "multi_waiters") == 0)
    return multiple_waiters();
  if (std::strcmp(argv[1], "wait_for") == 0)
    return wait_for_path();
  if (std::strcmp(argv[1], "void") == 0)
    return void_result();
  if (std::strcmp(argv[1], "exception") == 0)
    return ordinary_exception();
  return 65;
}
