#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstring>
#include <exception>
#include <mutex>
#include <pthread.h>
#include <ratio>
#include <unistd.h>

namespace {

std::mutex mutex;
std::condition_variable cv;
std::atomic<bool> ready{false};
std::atomic<int> cleanup_count{0};

enum class Mode { wait, system_clock, steady_clock, custom_clock };

const char* mode_name(Mode mode) {
  switch (mode) {
  case Mode::wait:
    return "wait";
  case Mode::system_clock:
    return "system_clock";
  case Mode::steady_clock:
    return "steady_clock";
  case Mode::custom_clock:
    return "custom_clock";
  }
  return "unknown";
}

struct CustomClock {
  using rep        = long long;
  using period     = std::ratio<1>;
  using duration   = std::chrono::duration<rep, period>;
  using time_point = std::chrono::time_point<CustomClock>;
  static constexpr bool is_steady = false;
  static time_point now() noexcept { return time_point(duration(0)); }
};

struct CleanupProbe {
  ~CleanupProbe() {
    cleanup_count.fetch_add(1, std::memory_order_relaxed);
    constexpr char marker[] = "event=cleanup\n";
    (void)!write(STDOUT_FILENO, marker, sizeof(marker) - 1);
  }
};

void terminate_marker() {
  char buffer[96];
  const int length = std::snprintf(buffer,
                                   sizeof(buffer),
                                   "event=terminate cleanup_count=%d\n",
                                   cleanup_count.load(std::memory_order_relaxed));
  if (length > 0)
    (void)!write(STDOUT_FILENO, buffer, static_cast<size_t>(length));
  _Exit(86);
}

void* worker(void* argument) {
  const Mode mode = *static_cast<Mode*>(argument);
  (void)pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr);
  (void)pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, nullptr);
  std::unique_lock<std::mutex> lock(mutex);
  CleanupProbe cleanup;
  ready.store(true, std::memory_order_release);
  std::printf("event=worker_ready mode=%s\n", mode_name(mode));
  std::fflush(stdout);

  switch (mode) {
  case Mode::wait:
    cv.wait(lock);
    break;
  case Mode::system_clock:
    cv.wait_until(lock, std::chrono::system_clock::now() + std::chrono::hours(1));
    break;
  case Mode::steady_clock:
    cv.wait_until(lock, std::chrono::steady_clock::now() + std::chrono::hours(1));
    break;
  case Mode::custom_clock:
    cv.wait_until(lock, CustomClock::now() + CustomClock::duration(3600));
    break;
  }

  std::printf("event=unexpected_return mode=%s\n", mode_name(mode));
  return reinterpret_cast<void*>(0x55);
}

bool parse_mode(const char* text, Mode& mode) {
  if (std::strcmp(text, "wait") == 0)
    mode = Mode::wait;
  else if (std::strcmp(text, "system_clock") == 0)
    mode = Mode::system_clock;
  else if (std::strcmp(text, "steady_clock") == 0)
    mode = Mode::steady_clock;
  else if (std::strcmp(text, "custom_clock") == 0)
    mode = Mode::custom_clock;
  else
    return false;
  return true;
}

} // namespace

int main(int argc, char** argv) {
  std::setvbuf(stdout, nullptr, _IONBF, 0);
  std::set_terminate(terminate_marker);
  Mode mode;
  if (argc != 2 || !parse_mode(argv[1], mode))
    return 64;

  pthread_t thread;
  const int create_rc = pthread_create(&thread, nullptr, worker, &mode);
  std::printf("event=create rc=%d\n", create_rc);
  if (create_rc != 0)
    return 65;

  while (!ready.load(std::memory_order_acquire))
    usleep(1000);

  mutex.lock();
  mutex.unlock();
  std::printf("event=wait_confirmed mode=%s\n", mode_name(mode));

  const int cancel_rc = pthread_cancel(thread);
  void* result        = nullptr;
  const int join_rc   = pthread_join(thread, &result);
  const bool canceled = result == PTHREAD_CANCELED;
  const int cleaned   = cleanup_count.load(std::memory_order_relaxed);
  std::printf("event=joined cancel_rc=%d join_rc=%d canceled=%d cleanup_count=%d\n",
              cancel_rc,
              join_rc,
              canceled ? 1 : 0,
              cleaned);
  return cancel_rc == 0 && join_rc == 0 && canceled && cleaned == 1 ? 0 : 66;
}
