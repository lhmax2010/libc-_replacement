#include <cstdio>
#include <pthread.h>
#include <unistd.h>

namespace {
struct Cleanup {
  ~Cleanup() { std::fprintf(stderr, "catchall:cleanup\n"); }
};
void* worker(void*) {
  try {
    Cleanup cleanup;
    std::fprintf(stderr, "catchall:ready\n");
    for (;;)
      sleep(1);
  } catch (...) {
    std::fprintf(stderr, "catchall:handler:rethrow\n");
    throw;
  }
}
}
int main() {
  pthread_t thread;
  if (pthread_create(&thread, nullptr, worker, nullptr) != 0)
    return 20;
  sleep(1);
  if (pthread_cancel(thread) != 0)
    return 21;
  void* result = nullptr;
  const int rc = pthread_join(thread, &result);
  std::fprintf(stderr, "catchall:join_rc=%d:canceled=%d:value=%p\n", rc,
               result == PTHREAD_CANCELED, result);
  return rc == 0 && result == PTHREAD_CANCELED ? 0 : 22;
}
