#include <atomic>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <future>
#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>

static std::atomic<bool> ready(false);

static void wait_until_ready() {
  for (int i = 0; i != 500 && !ready.load(std::memory_order_acquire); ++i)
    usleep(10000);
}

static int cancel_and_join(pthread_t thread, const char* mode) {
  wait_until_ready();
  for (int i = 0; i != 3; ++i) {
    int rc = pthread_cancel(thread);
    std::fprintf(stderr, "MODE=%s CANCEL_ATTEMPT=%d RC=%d\n", mode, i + 1, rc);
    usleep(10000);
  }
  void* result = nullptr;
  int join_rc = pthread_join(thread, &result);
  std::fprintf(stderr, "MODE=%s JOIN_RC=%d JOIN_IS_CANCELED=%d JOIN_VALUE=%p\n",
               mode, join_rc, result == PTHREAD_CANCELED, result);
  return join_rc == 0 && result == PTHREAD_CANCELED ? 0 : 2;
}

static void* cout_worker(void*) {
  ready.store(true, std::memory_order_release);
  for (unsigned long i = 0;; ++i)
    std::cout << i << std::endl;
  return nullptr;
}

static int run_cout() {
  int pipe_fds[2];
  if (pipe(pipe_fds) != 0)
    return 10;
  if (dup2(pipe_fds[1], STDOUT_FILENO) < 0)
    return 11;
  close(pipe_fds[1]);
  pthread_t thread;
  int create_rc = pthread_create(&thread, nullptr, cout_worker, nullptr);
  std::fprintf(stderr, "MODE=cout CREATE_RC=%d PIPE_READ_FD=%d\n", create_rc, pipe_fds[0]);
  if (create_rc != 0)
    return 12;
  return cancel_and_join(thread, "cout");
}

static void* getline_worker(void*) {
  ready.store(true, std::memory_order_release);
  std::string line;
  std::getline(std::cin, line);
  return nullptr;
}

static int run_getline() {
  int pipe_fds[2];
  if (pipe(pipe_fds) != 0)
    return 20;
  if (dup2(pipe_fds[0], STDIN_FILENO) < 0)
    return 21;
  close(pipe_fds[0]);
  pthread_t thread;
  int create_rc = pthread_create(&thread, nullptr, getline_worker, nullptr);
  std::fprintf(stderr, "MODE=getline CREATE_RC=%d PIPE_WRITE_FD=%d\n", create_rc, pipe_fds[1]);
  if (create_rc != 0)
    return 22;
  return cancel_and_join(thread, "getline");
}

static void packaged_callable() {
  ready.store(true, std::memory_order_release);
  for (;;)
    pthread_testcancel();
}

static void* packaged_worker(void* opaque) {
  std::packaged_task<void()>* task = static_cast<std::packaged_task<void()>*>(opaque);
  (*task)();
  return nullptr;
}

static int run_packaged_task() {
  std::packaged_task<void()> task(packaged_callable);
  pthread_t thread;
  int create_rc = pthread_create(&thread, nullptr, packaged_worker, &task);
  std::fprintf(stderr, "MODE=packaged_task CREATE_RC=%d\n", create_rc);
  if (create_rc != 0)
    return 32;
  return cancel_and_join(thread, "packaged_task");
}

static void* shrink_worker(void*) {
  std::string value(16384, 'x');
  ready.store(true, std::memory_order_release);
  for (;;) {
    value.resize(8192);
    value.shrink_to_fit();
    value.append(8192, 'x');
    pthread_testcancel();
  }
  return nullptr;
}

static int run_shrink() {
  pthread_t thread;
  int create_rc = pthread_create(&thread, nullptr, shrink_worker, nullptr);
  std::fprintf(stderr, "MODE=shrink_to_fit CREATE_RC=%d INTERNAL_CANCEL_POINT=0\n", create_rc);
  if (create_rc != 0)
    return 42;
  int rc = cancel_and_join(thread, "shrink_to_fit");
  std::fprintf(stderr, "SHRINK_INTERNAL_FORCED_UNWIND=NOT_OBSERVED\n");
  return rc;
}

static void* control_worker(void*) {
  volatile unsigned long value = 0;
  ready.store(true, std::memory_order_release);
  for (;;) {
    ++value;
    pthread_testcancel();
  }
  return nullptr;
}

static int run_control() {
  pthread_t thread;
  int create_rc = pthread_create(&thread, nullptr, control_worker, nullptr);
  std::fprintf(stderr, "MODE=control CREATE_RC=%d\n", create_rc);
  if (create_rc != 0)
    return 52;
  return cancel_and_join(thread, "control");
}

extern "C" __attribute__((visibility("default"))) int r52_run(int argc, char** argv) {
  if (argc != 2) {
    std::fprintf(stderr, "USAGE=%s cout|getline|packaged_task|shrink_to_fit|control\n", argv[0]);
    return 64;
  }
  ready.store(false, std::memory_order_release);
  if (std::strcmp(argv[1], "cout") == 0)
    return run_cout();
  if (std::strcmp(argv[1], "getline") == 0)
    return run_getline();
  if (std::strcmp(argv[1], "packaged_task") == 0)
    return run_packaged_task();
  if (std::strcmp(argv[1], "shrink_to_fit") == 0)
    return run_shrink();
  if (std::strcmp(argv[1], "control") == 0)
    return run_control();
  std::fprintf(stderr, "UNKNOWN_MODE=%s\n", argv[1]);
  return 65;
}
