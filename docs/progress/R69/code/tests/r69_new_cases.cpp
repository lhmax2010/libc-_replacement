#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <future>
#include <iostream>
#include <locale>
#include <pthread.h>
#include <sched.h>
#include <sstream>

namespace {

std::atomic<int> facet_entered{0};

class cancel_num_put : public std::num_put<char> {
protected:
  iter_type do_put(iter_type out, std::ios_base&, char_type, long) const override {
    facet_entered.store(1, std::memory_order_release);
    for (;;)
      pthread_testcancel();
    return out;
  }
};

struct StreamArgument {
  std::ostream* stream;
};

void* arithmetic_worker(void* raw) {
  int previous = 0;
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &previous);
  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &previous);
  StreamArgument* argument = static_cast<StreamArgument*>(raw);
  *argument->stream << 42L;
  return reinterpret_cast<void*>(1);
}

bool wait_until_at_least(const std::atomic<int>& value, int target) {
  for (int i = 0; i != 2000000; ++i) {
    if (value.load(std::memory_order_acquire) >= target)
      return true;
    sched_yield();
  }
  return false;
}

int arithmetic_state_case() {
  std::ostringstream stream;
  stream.imbue(std::locale(std::locale::classic(), new cancel_num_put));
  StreamArgument argument{&stream};
  pthread_t thread{};
  const int create_rc = pthread_create(&thread, nullptr, arithmetic_worker, &argument);
  if (create_rc != 0)
    return 10;
  if (!wait_until_at_least(facet_entered, 1))
    return 11;
  const int cancel_rc = pthread_cancel(thread);
  void* joined = nullptr;
  const int join_rc = pthread_join(thread, &joined);
  const int state = static_cast<int>(stream.rdstate());
  const bool canceled = joined == PTHREAD_CANCELED;
  const bool bad = (state & std::ios_base::badbit) != 0;
  std::cout << "CASE=arithmetic_state CREATE_RC=" << create_rc << " CANCEL_RC=" << cancel_rc
            << " JOIN_RC=" << join_rc << " JOIN_CANCELED=" << canceled << " RDSTATE=" << state
            << " BAD=" << bad << " FAIL=" << stream.fail() << '\n';
  return cancel_rc == 0 && join_rc == 0 && canceled && bad ? 0 : 12;
}

std::atomic<int> deferred_invocations{0};
std::shared_future<int>* deferred_future = nullptr;

int deferred_callable() {
  const int invocation = deferred_invocations.fetch_add(1, std::memory_order_acq_rel) + 1;
  if (invocation <= 2) {
    for (;;)
      pthread_testcancel();
  }
  return 42;
}

void* deferred_waiter(void*) {
  const int value = deferred_future->get();
  return reinterpret_cast<void*>(static_cast<intptr_t>(value));
}

int deferred_recancel_case() {
  std::shared_future<int> future = std::async(std::launch::deferred, deferred_callable).share();
  deferred_future                 = &future;
  bool canceled[2]                = {false, false};
  int cancel_rc[2]                = {-1, -1};
  int join_rc[2]                  = {-1, -1};
  for (int round = 0; round != 2; ++round) {
    pthread_t thread{};
    if (pthread_create(&thread, nullptr, deferred_waiter, nullptr) != 0)
      return 20 + round;
    if (!wait_until_at_least(deferred_invocations, round + 1))
      return 22 + round;
    cancel_rc[round] = pthread_cancel(thread);
    void* joined     = nullptr;
    join_rc[round]   = pthread_join(thread, &joined);
    canceled[round]  = joined == PTHREAD_CANCELED;
  }
  const int value = future.get();
  const int calls = deferred_invocations.load(std::memory_order_acquire);
  std::cout << "CASE=deferred_recancel CANCEL1_RC=" << cancel_rc[0] << " JOIN1_RC=" << join_rc[0]
            << " CANCELED1=" << canceled[0] << " CANCEL2_RC=" << cancel_rc[1]
            << " JOIN2_RC=" << join_rc[1] << " CANCELED2=" << canceled[1] << " INVOCATIONS=" << calls
            << " VALUE=" << value << '\n';
  return cancel_rc[0] == 0 && cancel_rc[1] == 0 && join_rc[0] == 0 && join_rc[1] == 0 &&
                 canceled[0] && canceled[1] && calls == 3 && value == 42
             ? 0
             : 24;
}

std::atomic<int> async_started{0};
std::atomic<int> async_release{0};
std::atomic<int> waiter_started{0};
std::shared_future<int>* waiting_future = nullptr;

int async_callable() {
  async_started.store(1, std::memory_order_release);
  while (!async_release.load(std::memory_order_acquire))
    sched_yield();
  return 42;
}

void* waiting_side(void*) {
  waiter_started.store(1, std::memory_order_release);
  waiting_future->wait();
  return reinterpret_cast<void*>(1);
}

int waiting_side_cancel_case() {
  std::shared_future<int> future = std::async(std::launch::async, async_callable).share();
  waiting_future                 = &future;
  if (!wait_until_at_least(async_started, 1))
    return 30;
  pthread_t waiter{};
  if (pthread_create(&waiter, nullptr, waiting_side, nullptr) != 0)
    return 31;
  if (!wait_until_at_least(waiter_started, 1))
    return 32;
  const int cancel_rc = pthread_cancel(waiter);
  void* joined        = nullptr;
  const int join_rc   = pthread_join(waiter, &joined);
  async_release.store(1, std::memory_order_release);
  const int value     = future.get();
  const bool canceled = joined == PTHREAD_CANCELED;
  std::cout << "CASE=waiting_side_cancel CANCEL_RC=" << cancel_rc << " JOIN_RC=" << join_rc
            << " JOIN_CANCELED=" << canceled << " VALUE=" << value << '\n';
  return cancel_rc == 0 && join_rc == 0 && canceled && value == 42 ? 0 : 33;
}

} // namespace

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: r69_new_cases arithmetic-state|deferred-recancel|waiting-side-cancel\n";
    return 64;
  }
  if (std::strcmp(argv[1], "arithmetic-state") == 0)
    return arithmetic_state_case();
  if (std::strcmp(argv[1], "deferred-recancel") == 0)
    return deferred_recancel_case();
  if (std::strcmp(argv[1], "waiting-side-cancel") == 0)
    return waiting_side_cancel_case();
  return 65;
}
