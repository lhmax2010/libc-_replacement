#include <atomic>
#include <cstdlib>
#include <future>
#include <iostream>
#include <pthread.h>
#include <system_error>

namespace {

std::atomic<int> value_move_constructed{0};
std::atomic<int> source_destructor_entered{0};

struct ExitAfterStore {
  bool trigger = true;

  ExitAfterStore() = default;
  ExitAfterStore(const ExitAfterStore&) = delete;
  ExitAfterStore& operator=(const ExitAfterStore&) = delete;

  ExitAfterStore(ExitAfterStore&& other) noexcept : trigger(false) {
    other.trigger = true;
    value_move_constructed.fetch_add(1, std::memory_order_relaxed);
  }

  ~ExitAfterStore() noexcept(false) {
    if (!trigger)
      return;
    source_destructor_entered.store(1, std::memory_order_release);
    pthread_exit(reinterpret_cast<void*>(0x60b));
  }
};

} // namespace

int main() {
  auto result = std::async(std::launch::async, [] { return ExitAfterStore{}; });

  try {
    (void)result.get();
    std::cout << "GET=VALUE\n";
    return 2;
  } catch (const std::future_error& error) {
    const bool broken = error.code() == std::make_error_code(std::future_errc::broken_promise);
    std::cout << "GET=FUTURE_ERROR CODE=" << error.code().value() << " BROKEN_PROMISE=" << broken
              << " VALUE_MOVE_CONSTRUCTED=" << value_move_constructed.load(std::memory_order_relaxed)
              << " SOURCE_DESTRUCTOR_ENTERED=" << source_destructor_entered.load(std::memory_order_acquire) << '\n';
    return broken && value_move_constructed.load(std::memory_order_relaxed) > 0 &&
                   source_destructor_entered.load(std::memory_order_acquire)
               ? 0
               : 3;
  } catch (...) {
    std::cout << "GET=OTHER_EXCEPTION\n";
    return 4;
  }
}
