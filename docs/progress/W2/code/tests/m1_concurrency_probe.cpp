#include <atomic>
#include <future>
#include <iostream>
#include <pthread.h>
#include <unistd.h>

namespace {

constexpr int waiter_count = 4;
std::atomic<int> callable_started{0};
std::atomic<int> invocations{0};
std::shared_future<int>* shared = nullptr;
pthread_t executor{};

void* wait_for_deferred(void*) {
  shared->wait();
  return reinterpret_cast<void*>(0x60);
}

} // namespace

int main() {
  auto f = std::async(std::launch::deferred, [] {
             const int current = ++invocations;
             if (current == 1) {
               executor = pthread_self();
               callable_started.store(1, std::memory_order_release);
               for (;;) {
                 pthread_testcancel();
                 usleep(1000);
               }
             }
             return 42;
           }).share();
  shared = &f;

  pthread_t waiters[waiter_count]{};
  for (int i = 0; i != waiter_count; ++i) {
    if (pthread_create(&waiters[i], nullptr, wait_for_deferred, nullptr) != 0)
      return 10;
  }
  while (!callable_started.load(std::memory_order_acquire))
    usleep(1000);

  const int cancel_rc = pthread_cancel(executor);
  int canceled_count  = 0;
  int returned_count  = 0;
  for (int i = 0; i != waiter_count; ++i) {
    void* result      = nullptr;
    const int join_rc = pthread_join(waiters[i], &result);
    if (join_rc != 0)
      return 11;
    if (result == PTHREAD_CANCELED)
      ++canceled_count;
    else if (result == reinterpret_cast<void*>(0x60))
      ++returned_count;
  }

  const int value = f.get();
  std::cout << "CANCEL_RC=" << cancel_rc << " CANCELED_COUNT=" << canceled_count
            << " RETURNED_COUNT=" << returned_count << " INVOCATIONS=" << invocations.load()
            << " VALUE=" << value << '\n';
  return cancel_rc == 0 && canceled_count == 1 && returned_count == waiter_count - 1 && invocations.load() == 2 &&
                 value == 42
             ? 0
             : 12;
}
