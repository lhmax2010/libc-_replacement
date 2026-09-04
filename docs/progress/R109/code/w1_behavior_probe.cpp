#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstring>
#include <exception>
#include <mutex>
#include <pthread.h>
#include <string>
#include <system_error>
#include <thread>
#include <unistd.h>

namespace {
void terminate_marker() {
  constexpr char marker[] = "event=terminate\n";
  (void)!write(STDOUT_FILENO, marker, sizeof(marker) - 1);
  _Exit(86);
}

int report_exception(const std::system_error& error) {
  std::printf("event=system_error code=%d category=%s message=%s\n",
              error.code().value(), error.code().category().name(), error.what());
  return 42;
}

int unowned_lock_case() {
  std::condition_variable condition;
  std::mutex mutex;
  std::unique_lock<std::mutex> lock(mutex, std::defer_lock);
  std::thread notifier([&] {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    condition.notify_one();
  });
  try {
    condition.wait(lock);
    notifier.join();
    const bool library_claims_ownership = lock.owns_lock();
    const bool native_was_available = mutex.try_lock();
    if (native_was_available) {
      mutex.unlock();
    } else if (!library_claims_ownership) {
      pthread_mutex_unlock(mutex.native_handle());
    }
    std::printf("event=returned owns_lock=%d native_try_lock=%s\n",
                library_claims_ownership ? 1 : 0,
                native_was_available ? "available" : "busy");
    return 0;
  } catch (const std::system_error& error) {
    notifier.join();
    return report_exception(error);
  } catch (...) {
    notifier.join();
    std::puts("event=other_exception");
    return 43;
  }
}

int backend_error_case() {
  std::condition_variable condition;
  std::mutex mutex;
  std::unique_lock<std::mutex> lock(mutex);
  try {
    condition.wait(lock);
    std::printf("event=returned owns_lock=%d\n", lock.owns_lock() ? 1 : 0);
    return 0;
  } catch (const std::system_error& error) {
    return report_exception(error);
  } catch (...) {
    std::puts("event=other_exception");
    return 43;
  }
}
}  // namespace

int main(int argc, char** argv) {
  std::set_terminate(terminate_marker);
  if (argc != 2) return 2;
  if (std::strcmp(argv[1], "unowned") == 0) return unowned_lock_case();
  if (std::strcmp(argv[1], "backend") == 0) return backend_error_case();
  return 2;
}
