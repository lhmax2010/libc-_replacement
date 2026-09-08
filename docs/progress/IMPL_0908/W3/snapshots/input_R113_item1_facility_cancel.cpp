#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstring>
#include <future>
#include <mutex>
#include <pthread.h>
#include <shared_mutex>
#include <stdexcept>
#include <unistd.h>

namespace {

using namespace std::chrono_literals;

std::atomic<int> ready{0};
std::atomic<int> cleanup_count{0};
std::atomic<int> unlock_seen{0};
std::timed_mutex timed;
std::__shared_mutex_base shared_base;
std::shared_timed_mutex shared_timed;
std::promise<int> promise;
std::future<int> future_value = promise.get_future();
std::condition_variable_any cv_any;
std::mutex user_mutex;

struct Cleanup {
  ~Cleanup() {
    cleanup_count.fetch_add(1, std::memory_order_relaxed);
    constexpr char marker[] = "event=cleanup\n";
    (void)!write(STDOUT_FILENO, marker, sizeof(marker) - 1);
  }
};

struct ThrowLock {
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  bool throw_on_lock = false;

  void lock() {
    if (throw_on_lock)
      throw std::runtime_error("ThrowLock::lock requested failure");
    const int rc = pthread_mutex_lock(&mutex);
    if (rc != 0)
      throw std::runtime_error("ThrowLock::lock pthread failure");
  }

  void unlock() {
    throw_on_lock = true;
    unlock_seen.store(1, std::memory_order_release);
    const int rc = pthread_mutex_unlock(&mutex);
    if (rc != 0)
      _Exit(92);
  }
};

ThrowLock throw_lock;

void terminate_marker() {
  constexpr char marker[] = "event=terminate\n";
  (void)!write(STDOUT_FILENO, marker, sizeof(marker) - 1);
  _Exit(86);
}

enum class Mode { timed_mutex, shared_mutex, shared_timed_mutex, future_wait, cv_any_wait, cv_any_throw };

const char* mode_name(Mode mode) {
  switch (mode) {
  case Mode::timed_mutex: return "timed_mutex";
  case Mode::shared_mutex: return "shared_mutex";
  case Mode::shared_timed_mutex: return "shared_timed_mutex";
  case Mode::future_wait: return "future_wait";
  case Mode::cv_any_wait: return "cv_any_wait";
  case Mode::cv_any_throw: return "cv_any_throw";
  }
  return "unknown";
}

bool parse_mode(const char* text, Mode& mode) {
  if (std::strcmp(text, "timed_mutex") == 0) mode = Mode::timed_mutex;
  else if (std::strcmp(text, "shared_mutex") == 0) mode = Mode::shared_mutex;
  else if (std::strcmp(text, "shared_timed_mutex") == 0) mode = Mode::shared_timed_mutex;
  else if (std::strcmp(text, "future_wait") == 0) mode = Mode::future_wait;
  else if (std::strcmp(text, "cv_any_wait") == 0) mode = Mode::cv_any_wait;
  else if (std::strcmp(text, "cv_any_throw") == 0) mode = Mode::cv_any_throw;
  else return false;
  return true;
}

void* worker(void* argument) {
  const Mode mode = *static_cast<Mode*>(argument);
  (void)pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr);
  (void)pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, nullptr);
  Cleanup cleanup;
  switch (mode) {
  case Mode::timed_mutex:
    ready.store(1, std::memory_order_release);
    timed.lock();
    timed.unlock();
    break;
  case Mode::shared_mutex:
    ready.store(1, std::memory_order_release);
    shared_base.lock();
    shared_base.unlock();
    break;
  case Mode::shared_timed_mutex:
    ready.store(1, std::memory_order_release);
    if (shared_timed.try_lock_for(1h))
      shared_timed.unlock();
    break;
  case Mode::future_wait:
    ready.store(1, std::memory_order_release);
    future_value.wait();
    break;
  case Mode::cv_any_wait: {
    std::unique_lock<std::mutex> lock(user_mutex);
    ready.store(1, std::memory_order_release);
    cv_any.wait(lock);
    break;
  }
  case Mode::cv_any_throw:
    throw_lock.lock();
    ready.store(1, std::memory_order_release);
    cv_any.wait(throw_lock);
    throw_lock.unlock();
    break;
  }
  constexpr char marker[] = "event=unexpected_return\n";
  (void)!write(STDOUT_FILENO, marker, sizeof(marker) - 1);
  return reinterpret_cast<void*>(0x55);
}

struct ContenderArgument {
  bool writer;
  std::atomic<int>* acquired;
};

void* shared_contender(void* opaque) {
  auto* argument = static_cast<ContenderArgument*>(opaque);
  if (argument->writer) {
    shared_base.lock();
    argument->acquired->store(1, std::memory_order_release);
    shared_base.unlock();
  } else {
    shared_base.lock_shared();
    argument->acquired->store(1, std::memory_order_release);
    shared_base.unlock_shared();
  }
  return nullptr;
}

int timed_join(pthread_t thread, int milliseconds) {
  timespec deadline{};
  clock_gettime(CLOCK_REALTIME, &deadline);
  deadline.tv_nsec += static_cast<long>(milliseconds) * 1000000L;
  deadline.tv_sec += deadline.tv_nsec / 1000000000L;
  deadline.tv_nsec %= 1000000000L;
  return pthread_timedjoin_np(thread, nullptr, &deadline);
}

bool blocked_shared_contender(bool writer) {
  std::atomic<int> acquired{0};
  ContenderArgument argument{writer, &acquired};
  pthread_t contender;
  if (pthread_create(&contender, nullptr, shared_contender, &argument) != 0)
    return false;
  const int join_rc = timed_join(contender, 300);
  const int cancel_rc = join_rc == ETIMEDOUT ? pthread_cancel(contender) : 0;
  void* result = nullptr;
  const int cleanup_join_rc = join_rc == ETIMEDOUT ? pthread_join(contender, &result) : 0;
  std::printf("event=contender writer=%d timed_join_rc=%d cancel_rc=%d cleanup_join_rc=%d acquired=%d canceled=%d\n",
              writer ? 1 : 0, join_rc, cancel_rc, cleanup_join_rc,
              acquired.load(std::memory_order_acquire), result == PTHREAD_CANCELED ? 1 : 0);
  return join_rc == ETIMEDOUT && cancel_rc == 0 && cleanup_join_rc == 0 && result == PTHREAD_CANCELED &&
         acquired.load(std::memory_order_acquire) == 0;
}

int run_mode(Mode mode) {
  if (mode == Mode::timed_mutex)
    timed.lock();
  else if (mode == Mode::shared_mutex)
    shared_base.lock_shared();
  else if (mode == Mode::shared_timed_mutex)
    shared_timed.lock_shared();

  pthread_t thread;
  const int create_rc = pthread_create(&thread, nullptr, worker, &mode);
  if (create_rc != 0)
    return 65;
  while (!ready.load(std::memory_order_acquire))
    usleep(1000);

  bool reached_second_gate = true;
  if (mode == Mode::shared_mutex) {
    reached_second_gate = false;
    for (int i = 0; i != 2000 && !reached_second_gate; ++i) {
      std::lock_guard<std::mutex> lock(shared_base.__mut_);
      reached_second_gate = (shared_base.__state_ & shared_base.__write_entered_) != 0;
      if (!reached_second_gate)
        usleep(1000);
    }
  } else if (mode == Mode::shared_timed_mutex) {
    reached_second_gate = false;
    for (int i = 0; i != 2000 && !reached_second_gate; ++i) {
      reached_second_gate = !shared_timed.try_lock_shared();
      if (!reached_second_gate) {
        shared_timed.unlock_shared();
        usleep(1000);
      }
    }
  } else if (mode == Mode::cv_any_throw) {
    for (int i = 0; i != 2000 && !unlock_seen.load(std::memory_order_acquire); ++i)
      usleep(1000);
    reached_second_gate = unlock_seen.load(std::memory_order_acquire) != 0;
  } else {
    usleep(100000);
  }
  std::printf("event=blocked mode=%s second_gate=%d\n", mode_name(mode), reached_second_gate ? 1 : 0);

  const int cancel_rc = pthread_cancel(thread);
  void* result = nullptr;
  const int join_rc = pthread_join(thread, &result);
  const bool canceled = result == PTHREAD_CANCELED;
  const int cleaned = cleanup_count.load(std::memory_order_acquire);
  std::printf("event=joined mode=%s cancel_rc=%d join_rc=%d canceled=%d cleanup=%d\n",
              mode_name(mode), cancel_rc, join_rc, canceled ? 1 : 0, cleaned);

  bool reusable = false;
  bool mutex_unlocked = true;
  bool state_flag = false;
  if (mode == Mode::timed_mutex) {
    timed.unlock();
    reusable = timed.try_lock_for(300ms);
    if (reusable)
      timed.unlock();
  } else if (mode == Mode::shared_mutex) {
    mutex_unlocked = shared_base.__mut_.try_lock();
    if (mutex_unlocked) {
      state_flag = (shared_base.__state_ & shared_base.__write_entered_) != 0;
      shared_base.__mut_.unlock();
    }
    shared_base.unlock_shared();
    const bool reader_blocked = blocked_shared_contender(false);
    const bool writer_blocked = blocked_shared_contender(true);
    reusable = !reader_blocked || !writer_blocked;
    std::printf("event=shared_state internal_mutex_unlocked=%d write_entered=%d reader_blocked=%d writer_blocked=%d\n",
                mutex_unlocked ? 1 : 0, state_flag ? 1 : 0, reader_blocked ? 1 : 0, writer_blocked ? 1 : 0);
    return reached_second_gate && cancel_rc == 0 && join_rc == 0 && canceled && cleaned == 1 &&
                   mutex_unlocked && state_flag && reader_blocked && writer_blocked
               ? 0
               : 67;
  } else if (mode == Mode::shared_timed_mutex) {
    shared_timed.unlock_shared();
    const bool reader = shared_timed.try_lock_shared_for(300ms);
    if (reader)
      shared_timed.unlock_shared();
    const bool writer = shared_timed.try_lock_for(300ms);
    if (writer)
      shared_timed.unlock();
    reusable = reader || writer;
    state_flag = !reader && !writer;
    std::printf("event=shared_timed_state reader_acquired=%d writer_acquired=%d write_entered_observed=%d\n",
                reader ? 1 : 0, writer ? 1 : 0, state_flag ? 1 : 0);
    return reached_second_gate && cancel_rc == 0 && join_rc == 0 && canceled && cleaned == 1 && state_flag ? 0 : 68;
  } else if (mode == Mode::future_wait) {
    promise.set_value(42);
    const bool ready_after = future_value.wait_for(300ms) == std::future_status::ready;
    const int value = ready_after ? future_value.get() : -1;
    reusable = ready_after && value == 42;
    std::printf("event=future_state ready=%d value=%d\n", ready_after ? 1 : 0, value);
  } else if (mode == Mode::cv_any_wait) {
    const bool user_mutex_unlocked = user_mutex.try_lock();
    if (user_mutex_unlocked)
      user_mutex.unlock();
    std::unique_lock<std::mutex> lock(user_mutex);
    const std::cv_status status = cv_any.wait_for(lock, 10ms);
    const bool cv_reused = status == std::cv_status::timeout && lock.owns_lock();
    reusable = user_mutex_unlocked && cv_reused;
    std::printf("event=cv_any_state user_mutex_unlocked=%d cv_reused=%d lock_owned=%d\n",
                user_mutex_unlocked ? 1 : 0, cv_reused ? 1 : 0, lock.owns_lock() ? 1 : 0);
  }

  std::printf("event=final mode=%s reusable=%d internal_mutex_unlocked=%d state_flag=%d\n",
              mode_name(mode), reusable ? 1 : 0, mutex_unlocked ? 1 : 0, state_flag ? 1 : 0);
  return reached_second_gate && cancel_rc == 0 && join_rc == 0 && canceled && cleaned == 1 && reusable ? 0 : 69;
}

} // namespace

int main(int argc, char** argv) {
  std::setvbuf(stdout, nullptr, _IONBF, 0);
  std::set_terminate(terminate_marker);
  Mode mode;
  if (argc != 2 || !parse_mode(argv[1], mode))
    return 64;
  return run_mode(mode);
}
