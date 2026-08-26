#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <pthread.h>
#include <ratio>
#include <unistd.h>

namespace {

std::mutex mutex;
std::condition_variable cv;
std::atomic<bool> ready{false};

enum class Mode { wait, wait_for, custom_clock };

struct CustomClock {
  using rep        = long long;
  using period     = std::ratio<1>;
  using duration   = std::chrono::duration<rep, period>;
  using time_point = std::chrono::time_point<CustomClock>;
  static constexpr bool is_steady = false;

  static time_point now() noexcept { return time_point(duration(0)); }
};

void* worker(void* argument) {
  const Mode mode = *static_cast<Mode*>(argument);
  std::unique_lock<std::mutex> lock(mutex);
  ready.store(true, std::memory_order_release);
  std::fprintf(stderr, "WORKER_ENTER mode=%d\n", static_cast<int>(mode));

  if (mode == Mode::wait)
    cv.wait(lock);
  else if (mode == Mode::wait_for)
    cv.wait_for(lock, std::chrono::hours(1));
  else
    cv.wait_until(lock, CustomClock::now() + CustomClock::duration(3600));

  std::fprintf(stderr, "WORKER_UNEXPECTED_RETURN\n");
  return reinterpret_cast<void*>(0x55);
}

} // namespace

int main(int argc, char** argv) {
  Mode mode;
  if (argc != 2) {
    std::fprintf(stderr, "USAGE: %s wait|wait_for|custom_clock\n", argv[0]);
    return 64;
  }
  if (std::strcmp(argv[1], "wait") == 0)
    mode = Mode::wait;
  else if (std::strcmp(argv[1], "wait_for") == 0)
    mode = Mode::wait_for;
  else if (std::strcmp(argv[1], "custom_clock") == 0)
    mode = Mode::custom_clock;
  else {
    std::fprintf(stderr, "UNKNOWN_MODE=%s\n", argv[1]);
    return 64;
  }

  pthread_t thread;
  const int create_rc = pthread_create(&thread, nullptr, worker, &mode);
  std::fprintf(stderr, "CREATE_RC=%d\n", create_rc);
  if (create_rc != 0)
    return 65;

  while (!ready.load(std::memory_order_acquire))
    usleep(1000);

  // The worker holds this mutex until condition_variable::wait* has entered
  // its blocking phase.  Acquiring it here proves cancellation is not racing
  // the diagnostic fprintf immediately before the wait.
  mutex.lock();
  mutex.unlock();
  std::fprintf(stderr, "WAIT_CONFIRMED\n");

  const int cancel_rc = pthread_cancel(thread);
  std::fprintf(stderr, "CANCEL_RC=%d\n", cancel_rc);
  void* result      = nullptr;
  const int join_rc = pthread_join(thread, &result);
  std::fprintf(stderr,
               "JOIN_RC=%d JOIN_CANCELED=%d RESULT=%p\n",
               join_rc,
               result == PTHREAD_CANCELED,
               result);
  return cancel_rc == 0 && join_rc == 0 && result == PTHREAD_CANCELED ? 0 : 66;
}
