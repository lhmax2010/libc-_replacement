#include <atomic>
#include <cstdio>
#include <new>
#include <pthread.h>
#include <string>
#include <unistd.h>

static std::atomic<bool> ready{false};

template <class T>
class CancelAllocator {
public:
  using value_type = T;

  CancelAllocator() noexcept = default;
  template <class U>
  CancelAllocator(const CancelAllocator<U>&) noexcept {}

  T* allocate(std::size_t count) {
    std::fprintf(stderr, "EVENT allocate count=%zu\n", count);
    ready.store(true, std::memory_order_release);
    for (;;)
      pthread_testcancel();
  }

  void deallocate(T* pointer, std::size_t) noexcept { ::operator delete(pointer); }

  template <class U>
  bool operator==(const CancelAllocator<U>&) const noexcept { return true; }
  template <class U>
  bool operator!=(const CancelAllocator<U>&) const noexcept { return false; }
};

using CancelString =
    std::basic_string<char, std::char_traits<char>, CancelAllocator<char>>;

static void* worker(void*) {
  CancelString value(1024, 'x');
  value.reserve(65536);
  value.shrink_to_fit();
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
  return joined == PTHREAD_CANCELED ? 0 : 1;
}
