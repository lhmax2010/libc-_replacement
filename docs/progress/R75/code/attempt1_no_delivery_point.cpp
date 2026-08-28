#include <atomic>
#include <cstdio>
#include <pthread.h>
#include <unistd.h>

static std::atomic<bool> ready{false};

static void* worker(void*) {
  ready.store(true, std::memory_order_release);
  for (;;)
    asm volatile("" ::: "memory");
  return nullptr;
}

int main() {
  pthread_t thread{};
  const int create_rc = pthread_create(&thread, nullptr, worker, nullptr);
  std::fprintf(stderr, "CREATE_RC=%d\n", create_rc);
  if (create_rc != 0)
    return 2;
  while (!ready.load(std::memory_order_acquire))
    usleep(1000);
  const int cancel_rc = pthread_cancel(thread);
  std::fprintf(stderr, "REQUEST_RC=%d\n", cancel_rc);
  void* joined = nullptr;
  const int join_rc = pthread_join(thread, &joined);
  std::fprintf(stderr, "JOIN_RC=%d JOINED=%s\n", join_rc,
               joined == PTHREAD_CANCELED ? "PTHREAD_CANCELED" : "other");
  return 0;
}
