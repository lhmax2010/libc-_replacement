#include <atomic>
#include <cstdio>
#include <cstring>
#include <future>
#include <iostream>
#include <pthread.h>
#include <streambuf>
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
  std::fprintf(stderr,
               "MODE=%s JOIN_RC=%d JOIN_IS_CANCELED=%d JOIN_VALUE=%p\n",
               mode,
               join_rc,
               result == PTHREAD_CANCELED,
               result);
  return join_rc == 0 && result == PTHREAD_CANCELED ? 0 : 2;
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
  std::fprintf(stderr,
               "MODE=getline CREATE_RC=%d PIPE_WRITE_FD=%d\n",
               create_rc,
               pipe_fds[1]);
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
  std::packaged_task<void()>* task =
      static_cast<std::packaged_task<void()>*>(opaque);
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

class CancelStreambuf : public std::streambuf {
protected:
  int_type underflow() override {
    ready.store(true, std::memory_order_release);
    for (;;)
      pthread_testcancel();
  }
};

static void* failbit_worker(void*) {
  CancelStreambuf source;
  std::cout << &source;
  return nullptr;
}

static int run_failbit_streambuf() {
  pthread_t thread;
  int create_rc = pthread_create(&thread, nullptr, failbit_worker, nullptr);
  std::fprintf(stderr, "MODE=failbit_streambuf CREATE_RC=%d\n", create_rc);
  if (create_rc != 0)
    return 42;
  return cancel_and_join(thread, "failbit_streambuf");
}

extern "C" int r46_run(const char* mode) {
  ready.store(false, std::memory_order_release);
  if (std::strcmp(mode, "getline") == 0)
    return run_getline();
  if (std::strcmp(mode, "packaged_task") == 0)
    return run_packaged_task();
  if (std::strcmp(mode, "failbit_streambuf") == 0)
    return run_failbit_streambuf();
  std::fprintf(stderr, "UNKNOWN_MODE=%s\n", mode);
  return 65;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::fprintf(stderr,
                 "USAGE=%s getline|packaged_task|failbit_streambuf\n",
                 argv[0]);
    return 64;
  }
  return r46_run(argv[1]);
}
