#include <pthread.h>
#include <unistd.h>

#include <cstdio>
#include <cxxabi.h>

struct Cleanup {
  const char* name;
  ~Cleanup() { std::fprintf(stderr, "cleanup:%s\n", name); }
};

static void* worker(void*) {
  try {
    Cleanup body{"body"};
    std::fprintf(stderr, "ready\n");
    for (;;)
      sleep(1);
  } catch (const __cxxabiv1::__forced_unwind&) {
    std::fprintf(stderr, "forced-handler\n");
    Cleanup handler{"handler"};
    throw;
  } catch (...) {
    std::fprintf(stderr, "catch-all\n");
  }
  return reinterpret_cast<void*>(1);
}

int main() {
  pthread_t thread;
  if (pthread_create(&thread, nullptr, worker, nullptr) != 0)
    return 1;
  sleep(1);
  if (pthread_cancel(thread) != 0)
    return 2;
  void* result = nullptr;
  if (pthread_join(thread, &result) != 0)
    return 3;
  std::fprintf(stderr, "join=%s\n",
               result == PTHREAD_CANCELED ? "PTHREAD_CANCELED" : "OTHER");
  return result == PTHREAD_CANCELED ? 0 : 4;
}
