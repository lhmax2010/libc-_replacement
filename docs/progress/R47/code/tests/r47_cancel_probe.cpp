// R47 取消实验探针:两个模式,分别阻塞在 basic_istream<char> 的两个
// 显式实例化成员内(库内代码),然后 pthread_cancel。
//   getline_member -> cin.getline(buf, n, '\n')  (istream:808 站点,库内)
//   read_member    -> cin.read(buf, n)           (istream:915 站点,库内)
// 纪律:线程函数非 noexcept;裸 pthread_create;应用代码无任何 catch;
// 取消点为 read(2)(线程阻塞于空管道)。
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
  usleep(200000); // 让线程真正进入阻塞的库调用
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

static void* getline_member_worker(void*) {
  char buf[256];
  ready.store(true, std::memory_order_release);
  std::cin.getline(buf, sizeof buf, '\n'); // 显式三参:istream:808 所在成员
  return nullptr;
}

static void* read_member_worker(void*) {
  char buf[16];
  ready.store(true, std::memory_order_release);
  std::cin.read(buf, sizeof buf);          // istream:915 所在成员
  return nullptr;
}

static int run_mode(void* (*worker)(void*), const char* mode) {
  int pipe_fds[2];
  if (pipe(pipe_fds) != 0)
    return 20;
  if (dup2(pipe_fds[0], STDIN_FILENO) < 0)
    return 21;
  close(pipe_fds[0]);
  pthread_t thread;
  int create_rc = pthread_create(&thread, nullptr, worker, nullptr);
  std::fprintf(stderr, "MODE=%s CREATE_RC=%d PIPE_WRITE_FD=%d\n", mode,
               create_rc, pipe_fds[1]);
  if (create_rc != 0)
    return 22;
  return cancel_and_join(thread, mode);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::fprintf(stderr, "USAGE=%s getline_member|read_member\n", argv[0]);
    return 64;
  }
  if (std::strcmp(argv[1], "getline_member") == 0)
    return run_mode(getline_member_worker, "getline_member");
  if (std::strcmp(argv[1], "read_member") == 0)
    return run_mode(read_member_worker, "read_member");
  std::fprintf(stderr, "UNKNOWN_MODE=%s\n", argv[1]);
  return 65;
}
