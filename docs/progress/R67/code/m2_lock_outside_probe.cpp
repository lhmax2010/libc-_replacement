#include <atomic>
#include <chrono>
#include <cstring>
#include <future>
#include <iostream>
#include <pthread.h>
#include <unistd.h>

namespace {

std::atomic<int> source_destructor_entered{0};
std::atomic<int> stored_destructor_entered{0};
std::atomic<int> stored_destructor_completed{0};
std::atomic<void*> state_future{nullptr};
pthread_t worker_thread{};

struct ReentrantResult {
  bool source_object = true;

  ReentrantResult() = default;
  ReentrantResult(const ReentrantResult&) = delete;
  ReentrantResult& operator=(const ReentrantResult&) = delete;
  ReentrantResult(ReentrantResult&& other) noexcept : source_object(false) { other.source_object = true; }

  ~ReentrantResult() noexcept(false);
};

ReentrantResult::~ReentrantResult() noexcept(false) {
  if (source_object) {
    source_destructor_entered.store(1, std::memory_order_release);
    for (;;)
      pthread_testcancel();
  }

  stored_destructor_entered.store(1, std::memory_order_release);
  void* state = nullptr;
  while ((state = state_future.load(std::memory_order_acquire)) == nullptr)
    usleep(1000);
  auto* observer = static_cast<std::shared_future<ReentrantResult>*>(state);
  const std::future_status status = observer->wait_for(std::chrono::milliseconds(0));
  std::cout << "STORED_DTOR_WAIT_STATUS=" << (status == std::future_status::timeout ? "TIMEOUT" : "OTHER") << '\n';
  stored_destructor_completed.store(status == std::future_status::timeout, std::memory_order_release);
}

ReentrantResult make_result() {
  worker_thread = pthread_self();
  int old_type = 0;
  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &old_type);
  return ReentrantResult{};
}

bool broken_promise(const std::future_error& error) {
  return error.code() == std::make_error_code(std::future_errc::broken_promise);
}

int run_probe() {
  auto result = std::async(std::launch::async, make_result).share();
  state_future.store(&result, std::memory_order_release);
  while (!source_destructor_entered.load(std::memory_order_acquire))
    usleep(1000);
  const int cancel_rc = pthread_cancel(worker_thread);
  bool got_broken = false;
  try {
    (void)result.get();
  } catch (const std::future_error& error) {
    got_broken = broken_promise(error);
    std::cout << "FUTURE_ERROR=" << error.what() << " BROKEN=" << got_broken << '\n';
  }
  std::cout << "CANCEL_RC=" << cancel_rc
            << " STORED_DTOR_ENTERED=" << stored_destructor_entered.load()
            << " STORED_DTOR_COMPLETED=" << stored_destructor_completed.load() << '\n';
  const bool destructor_did_not_block =
      !stored_destructor_entered.load() || stored_destructor_completed.load();
  return cancel_rc == 0 && got_broken && destructor_did_not_block ? 0 : 2;
}

} // namespace

int main() { return run_probe(); }
