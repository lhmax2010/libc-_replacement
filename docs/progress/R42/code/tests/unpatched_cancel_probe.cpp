#include <pthread.h>
#include <unistd.h>

#include <cstdio>

namespace {

struct Cleanup {
  ~Cleanup() { std::fprintf(stderr, "unpatched:cleanup\n"); }
};

void* worker(void*) {
  try {
    Cleanup cleanup;
    std::fprintf(stderr, "unpatched:ready\n");
    for (;;)
      ::sleep(1);
  } catch (...) {
    std::fprintf(stderr, "unpatched:catch-all\n");
  }
  return reinterpret_cast<void*>(0x11);
}

} // namespace

int main() {
  pthread_t thread;
  if (::pthread_create(&thread, nullptr, worker, nullptr) != 0)
    return 20;
  ::sleep(1);
  if (::pthread_cancel(thread) != 0)
    return 21;
  void* result = nullptr;
  if (::pthread_join(thread, &result) != 0)
    return 22;
  std::fprintf(stderr, "unpatched:join=%s\n",
               result == PTHREAD_CANCELED ? "PTHREAD_CANCELED" : "OTHER");
  return result == PTHREAD_CANCELED ? 0 : 23;
}
