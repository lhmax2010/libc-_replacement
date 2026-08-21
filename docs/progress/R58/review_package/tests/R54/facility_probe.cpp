#include <atomic>
#include <cstdio>
#include <cstring>
#include <future>
#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>

namespace {
std::atomic<bool> ready{false};
std::atomic<unsigned long> async_thread{0};

int cancel_join(pthread_t thread, const char* mode) {
  for (int i = 0; i < 500 && !ready.load(std::memory_order_acquire); ++i)
    usleep(10000);
  const int cancel_rc = pthread_cancel(thread);
  void* result = nullptr;
  const int join_rc = pthread_join(thread, &result);
  std::fprintf(stderr, "MODE=%s CANCEL_RC=%d JOIN_RC=%d JOIN_IS_CANCELED=%d JOIN_VALUE=%p\n",
               mode, cancel_rc, join_rc, result == PTHREAD_CANCELED, result);
  return cancel_rc == 0 && join_rc == 0 && result == PTHREAD_CANCELED ? 0 : 2;
}

void* cout_worker(void*) {
  ready.store(true, std::memory_order_release);
  for (unsigned long i = 0;; ++i)
    std::cout << i << std::endl;
}

void* input_worker(void* opaque) {
  const char* mode = static_cast<const char*>(opaque);
  ready.store(true, std::memory_order_release);
  if (std::strcmp(mode, "getline_free") == 0) {
    std::string line;
    std::getline(std::cin, line);
  } else if (std::strcmp(mode, "getline_member") == 0) {
    char buffer[128];
    std::cin.getline(buffer, sizeof(buffer));
  } else if (std::strcmp(mode, "read") == 0) {
    char buffer[128];
    std::cin.read(buffer, sizeof(buffer));
  } else {
    int value = 0;
    std::cin >> value;
  }
  return nullptr;
}

void packaged_callable() {
  ready.store(true, std::memory_order_release);
  for (;;)
    pthread_testcancel();
}

void* packaged_worker(void* opaque) {
  (*static_cast<std::packaged_task<void()>*>(opaque))();
  return nullptr;
}

int run_async() {
  auto future = std::async(std::launch::async, [] {
    async_thread.store(static_cast<unsigned long>(pthread_self()), std::memory_order_release);
    ready.store(true, std::memory_order_release);
    for (;;)
      pthread_testcancel();
  });
  for (int i = 0; i < 500 && !ready.load(std::memory_order_acquire); ++i)
    usleep(10000);
  const pthread_t thread = static_cast<pthread_t>(async_thread.load(std::memory_order_acquire));
  const int cancel_rc = pthread_cancel(thread);
  std::fprintf(stderr, "MODE=async CANCEL_RC=%d THREAD=%lu\n", cancel_rc,
               static_cast<unsigned long>(thread));
  future.wait();
  std::fprintf(stderr, "MODE=async FUTURE_WAIT_RETURNED=1\n");
  return cancel_rc == 0 ? 0 : 3;
}
} // namespace

int main(int argc, char** argv) {
  if (argc != 2)
    return 64;
  const char* mode = argv[1];
  ready.store(false, std::memory_order_release);
  async_thread.store(0, std::memory_order_release);
  if (std::strcmp(mode, "async") == 0)
    return run_async();
  int pipe_fds[2];
  if (pipe(pipe_fds) != 0)
    return 10;
  if (std::strcmp(mode, "cout") == 0) {
    if (dup2(pipe_fds[1], STDOUT_FILENO) < 0)
      return 11;
    close(pipe_fds[1]);
    pthread_t thread;
    const int rc = pthread_create(&thread, nullptr, cout_worker, nullptr);
    std::fprintf(stderr, "MODE=cout CREATE_RC=%d\n", rc);
    return rc == 0 ? cancel_join(thread, mode) : 12;
  }
  if (std::strcmp(mode, "packaged_task") == 0) {
    std::packaged_task<void()> task(packaged_callable);
    pthread_t thread;
    const int rc = pthread_create(&thread, nullptr, packaged_worker, &task);
    std::fprintf(stderr, "MODE=packaged_task CREATE_RC=%d\n", rc);
    return rc == 0 ? cancel_join(thread, mode) : 13;
  }
  if (std::strcmp(mode, "getline_free") != 0 &&
      std::strcmp(mode, "getline_member") != 0 &&
      std::strcmp(mode, "read") != 0 && std::strcmp(mode, "arithmetic") != 0)
    return 65;
  if (dup2(pipe_fds[0], STDIN_FILENO) < 0)
    return 14;
  close(pipe_fds[0]);
  pthread_t thread;
  const int rc = pthread_create(&thread, nullptr, input_worker, argv[1]);
  std::fprintf(stderr, "MODE=%s CREATE_RC=%d\n", mode, rc);
  return rc == 0 ? cancel_join(thread, mode) : 15;
}
