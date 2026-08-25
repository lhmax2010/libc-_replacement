#include <atomic>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <pthread.h>
#include <stdexcept>
#include <unistd.h>

namespace {
constexpr int kThreads = 4;
std::atomic<int> ready{0};

void* cancel_point_worker(void*) {
  ready.fetch_add(1, std::memory_order_release);
  for (;;)
    pthread_testcancel();
}

void* cout_worker(void*) {
  ready.fetch_add(1, std::memory_order_release);
  for (unsigned long i = 0;; ++i)
    std::cout << i << std::endl;
}

void* exception_worker(void* opaque) {
  const long id = reinterpret_cast<long>(opaque);
  try {
    throw std::runtime_error("concurrent-real-exception");
  } catch (const std::runtime_error& error) {
    std::fprintf(stderr, "EXCEPTION_THREAD=%ld CAUGHT=%s\n", id, error.what());
  }
  return reinterpret_cast<void*>(0x42);
}

int wait_ready(int expected) {
  for (int i = 0; i < 1000 && ready.load(std::memory_order_acquire) < expected; ++i)
    usleep(10000);
  return ready.load(std::memory_order_acquire) == expected ? 0 : 70;
}

int cancel_and_join_all(pthread_t* threads, int count, bool repeat) {
  int failures = 0;
  for (int i = 0; i < count; ++i) {
    const int first = pthread_cancel(threads[i]);
    const int second = repeat ? pthread_cancel(threads[i]) : -1;
    std::fprintf(stderr, "THREAD=%d CANCEL1=%d CANCEL2=%d\n", i, first, second);
    failures += first != 0;
    failures += repeat && second != 0;
  }
  for (int i = 0; i < count; ++i) {
    void* result = nullptr;
    const int rc = pthread_join(threads[i], &result);
    std::fprintf(stderr, "THREAD=%d JOIN_RC=%d JOIN_IS_CANCELED=%d JOIN_VALUE=%p\n",
                 i, rc, result == PTHREAD_CANCELED, result);
    failures += rc != 0 || result != PTHREAD_CANCELED;
  }
  return failures == 0 ? 0 : 71;
}

int run_cancel_group(void* (*worker)(void*), bool repeat) {
  pthread_t threads[kThreads];
  ready.store(0, std::memory_order_release);
  for (int i = 0; i < kThreads; ++i)
    if (pthread_create(&threads[i], nullptr, worker, nullptr) != 0)
      return 72;
  if (wait_ready(kThreads) != 0)
    return 73;
  return cancel_and_join_all(threads, kThreads, repeat);
}

int run_cancel_and_exception() {
  pthread_t canceled[kThreads];
  pthread_t exceptions[kThreads];
  ready.store(0, std::memory_order_release);
  for (int i = 0; i < kThreads; ++i) {
    if (pthread_create(&canceled[i], nullptr, cancel_point_worker, nullptr) != 0)
      return 74;
    if (pthread_create(&exceptions[i], nullptr, exception_worker,
                       reinterpret_cast<void*>(static_cast<long>(i))) != 0)
      return 75;
  }
  if (wait_ready(kThreads) != 0)
    return 76;
  int rc = cancel_and_join_all(canceled, kThreads, false);
  for (int i = 0; i < kThreads; ++i) {
    void* result = nullptr;
    const int join_rc = pthread_join(exceptions[i], &result);
    std::fprintf(stderr, "EXCEPTION_THREAD=%d JOIN_RC=%d VALUE=%p\n", i, join_rc, result);
    if (join_rc != 0 || result != reinterpret_cast<void*>(0x42))
      rc = 77;
  }
  return rc;
}
} // namespace

int main(int argc, char** argv) {
  if (argc != 2)
    return 64;
  if (std::strcmp(argv[1], "multi_cancel") == 0)
    return run_cancel_group(cancel_point_worker, false);
  if (std::strcmp(argv[1], "cancel_and_exception") == 0)
    return run_cancel_and_exception();
  if (std::strcmp(argv[1], "stdlib_concurrent") == 0) {
    int pipe_fds[2];
    if (pipe(pipe_fds) != 0 || dup2(pipe_fds[1], STDOUT_FILENO) < 0)
      return 78;
    close(pipe_fds[1]);
    return run_cancel_group(cout_worker, false);
  }
  if (std::strcmp(argv[1], "repeat_cancel") == 0)
    return run_cancel_group(cancel_point_worker, true);
  return 65;
}
