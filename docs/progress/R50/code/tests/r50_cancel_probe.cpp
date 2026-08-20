// R50 ARM 原生对照探针：命中两个经 libc++.so 显式实例化交付的
// istream 站点。线程函数非 noexcept；裸 pthread_create；应用层无 catch。
#include <atomic>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <pthread.h>
#include <unistd.h>

static std::atomic<bool> ready(false);

static void wait_until_ready() {
  for (int i = 0; i != 500 && !ready.load(std::memory_order_acquire); ++i)
    usleep(10000);
}

static int cancel_and_join(pthread_t thread, const char* mode) {
  wait_until_ready();
  usleep(200000);
  for (int i = 0; i != 3; ++i) {
    int rc = pthread_cancel(thread);
    std::fprintf(stderr, "MODE=%s CANCEL_ATTEMPT=%d RC=%d\n", mode, i + 1, rc);
    usleep(10000);
  }
  void* result = nullptr;
  int join_rc = pthread_join(thread, &result);
  std::fprintf(stderr,
               "MODE=%s JOIN_RC=%d JOIN_IS_CANCELED=%d JOIN_VALUE=%p\n",
               mode, join_rc, result == PTHREAD_CANCELED, result);
  return join_rc == 0 && result == PTHREAD_CANCELED ? 0 : 2;
}

static void* getline_member_worker(void*) {
  char buf[256];
  ready.store(true, std::memory_order_release);
  std::cin.getline(buf, sizeof buf, '\n'); // S05, istream:808
  return nullptr;
}

static void* read_member_worker(void*) {
  char buf[16];
  ready.store(true, std::memory_order_release);
  std::cin.read(buf, sizeof buf); // S08, istream:915
  return nullptr;
}

static int run_mode(void* (*worker)(void*), const char* mode) {
  ready.store(false, std::memory_order_release);
  int pipe_fds[2];
  if (pipe(pipe_fds) != 0)
    return 20;
  if (dup2(pipe_fds[0], STDIN_FILENO) < 0)
    return 21;
  close(pipe_fds[0]);
  pthread_t thread;
  int create_rc = pthread_create(&thread, nullptr, worker, nullptr);
  std::fprintf(stderr, "MODE=%s CREATE_RC=%d PIPE_WRITE_FD=%d\n",
               mode, create_rc, pipe_fds[1]);
  if (create_rc != 0)
    return 22;
  return cancel_and_join(thread, mode);
}

// DSO launcher 用 dlsym 查找此入口；生产式 -fvisibility=hidden 下仅显式
// 导出测试入口，不改变被测标准库调用路径。
extern "C" __attribute__((visibility("default"))) int r50_run(const char* mode) {
  if (std::strcmp(mode, "getline_member") == 0)
    return run_mode(getline_member_worker, "getline_member");
  if (std::strcmp(mode, "read_member") == 0)
    return run_mode(read_member_worker, "read_member");
  std::fprintf(stderr, "UNKNOWN_MODE=%s\n", mode);
  return 65;
}
