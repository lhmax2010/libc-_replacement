#include <cstdio>
#include <iostream>
#include <pthread.h>
#include <streambuf>
#include <unistd.h>

static int ready_pipe[2];

class CancelStreambuf : public std::streambuf {
protected:
  int_type underflow() override {
    const char marker = 'R';
    (void)write(ready_pipe[1], &marker, 1);
    for (;;)
      pthread_testcancel();
  }
};

static void* worker(void*) {
  CancelStreambuf source;
  std::cout << &source;
  return nullptr;
}

extern "C" __attribute__((visibility("default"))) int r52_run(int, char**) {
  if (pipe(ready_pipe) != 0)
    return 10;
  pthread_t thread;
  int create_rc = pthread_create(&thread, nullptr, worker, nullptr);
  std::fprintf(stderr, "MODE=failbit_streambuf CREATE_RC=%d\n", create_rc);
  if (create_rc != 0)
    return 11;
  char marker = 0;
  if (read(ready_pipe[0], &marker, 1) != 1)
    return 12;
  for (int i = 0; i != 3; ++i) {
    int rc = pthread_cancel(thread);
    std::fprintf(stderr,
                 "MODE=failbit_streambuf CANCEL_ATTEMPT=%d RC=%d\n",
                 i + 1,
                 rc);
    usleep(10000);
  }
  void* result = nullptr;
  int join_rc = pthread_join(thread, &result);
  std::fprintf(stderr,
               "MODE=failbit_streambuf JOIN_RC=%d JOIN_IS_CANCELED=%d JOIN_VALUE=%p\n",
               join_rc,
               result == PTHREAD_CANCELED,
               result);
  return join_rc == 0 && result == PTHREAD_CANCELED ? 0 : 2;
}
