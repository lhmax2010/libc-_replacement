#include <atomic>
#include <cerrno>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <dlfcn.h>
#include <mutex>
#include <pthread.h>
#include <unistd.h>

namespace {

std::mutex target_mutex;
std::condition_variable target_cv;
std::atomic<bool> ready{false};
std::atomic<bool> track_unlock{false};
std::atomic<int> target_unlock_count{0};
std::atomic<int> frame_cleanup_count{0};
pthread_mutex_t* target_native = nullptr;
using UnlockFn = int (*)(pthread_mutex_t*);
UnlockFn real_unlock = nullptr;

struct FrameCleanup {
  ~FrameCleanup() {
    frame_cleanup_count.fetch_add(1, std::memory_order_relaxed);
    constexpr char marker[] = "event=frame_cleanup\n";
    (void)!write(STDOUT_FILENO, marker, sizeof(marker) - 1);
  }
};

void terminate_marker() {
  char buffer[128];
  int length = std::snprintf(buffer,
                             sizeof(buffer),
                             "event=terminate frame_cleanup=%d target_unlock=%d\n",
                             frame_cleanup_count.load(std::memory_order_relaxed),
                             target_unlock_count.load(std::memory_order_relaxed));
  if (length > 0)
    (void)!write(STDOUT_FILENO, buffer, static_cast<size_t>(length));
  _Exit(86);
}

void* worker(void*) {
  (void)pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr);
  (void)pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, nullptr);
  std::unique_lock<std::mutex> lock(target_mutex);
  FrameCleanup cleanup;
  ready.store(true, std::memory_order_release);
#ifndef WORKER_NO_PUTS
  std::puts("event=worker_ready");
#endif
  target_cv.wait(lock);
#ifndef WORKER_NO_PUTS
  std::puts("event=unexpected_return");
#endif
  return reinterpret_cast<void*>(0x55);
}

void* contender(void*) {
  timespec deadline{};
  clock_gettime(CLOCK_REALTIME, &deadline);
  deadline.tv_nsec += 300000000;
  if (deadline.tv_nsec >= 1000000000) {
    deadline.tv_sec += 1;
    deadline.tv_nsec -= 1000000000;
  }
  int rc = pthread_mutex_timedlock(target_native, &deadline);
  std::printf("event=contender rc=%d errno_busy=%d errno_timeout=%d\n", rc, EBUSY, ETIMEDOUT);
  if (rc == 0)
    real_unlock(target_native);
  return reinterpret_cast<void*>(static_cast<intptr_t>(rc));
}

} // namespace

extern "C" int pthread_mutex_unlock(pthread_mutex_t* mutex) {
  if (real_unlock == nullptr)
    real_unlock = reinterpret_cast<UnlockFn>(dlsym(RTLD_NEXT, "pthread_mutex_unlock"));
  if (track_unlock.load(std::memory_order_relaxed) && mutex == target_native)
    target_unlock_count.fetch_add(1, std::memory_order_relaxed);
  if (real_unlock == nullptr)
    _Exit(87);
  return real_unlock(mutex);
}

int main() {
  std::setvbuf(stdout, nullptr, _IONBF, 0);
  std::set_terminate(terminate_marker);
  real_unlock = reinterpret_cast<UnlockFn>(dlsym(RTLD_NEXT, "pthread_mutex_unlock"));
  if (real_unlock == nullptr) {
    std::puts("event=dlsym_failed");
    return 68;
  }
  target_native = target_mutex.native_handle();

  pthread_t thread;
  int create_rc = pthread_create(&thread, nullptr, worker, nullptr);
  std::printf("event=create rc=%d\n", create_rc);
  if (create_rc != 0)
    return 65;
  while (!ready.load(std::memory_order_acquire))
    usleep(1000);

  std::puts("event=worker_ready_observed");

  target_mutex.lock();
  target_mutex.unlock();
  std::puts("event=wait_confirmed");
  track_unlock.store(true, std::memory_order_release);
  int cancel_rc = pthread_cancel(thread);
  void* result = nullptr;
  int join_rc = pthread_join(thread, &result);
  track_unlock.store(false, std::memory_order_release);
  int unlocks = target_unlock_count.load(std::memory_order_relaxed);
  int cleanups = frame_cleanup_count.load(std::memory_order_relaxed);
  std::printf("event=joined cancel_rc=%d join_rc=%d canceled=%d frame_cleanup=%d target_unlock=%d\n",
              cancel_rc,
              join_rc,
              result == PTHREAD_CANCELED ? 1 : 0,
              cleanups,
              unlocks);

  int try_rc = pthread_mutex_trylock(target_native);
  std::printf("event=trylock rc=%d errno_busy=%d\n", try_rc, EBUSY);
  if (try_rc == 0)
    real_unlock(target_native);

  pthread_t contender_thread;
  int contender_create_rc = pthread_create(&contender_thread, nullptr, contender, nullptr);
  void* contender_result = nullptr;
  int contender_join_rc = contender_create_rc == 0 ? pthread_join(contender_thread, &contender_result) : -1;
  int contender_rc = static_cast<int>(reinterpret_cast<intptr_t>(contender_result));
  std::printf("event=contender_join create_rc=%d join_rc=%d contender_rc=%d\n",
              contender_create_rc,
              contender_join_rc,
              contender_rc);

  bool joined_canceled = cancel_rc == 0 && join_rc == 0 && result == PTHREAD_CANCELED;
#if EXPECT_CLEAN > 0
  bool expected = joined_canceled && cleanups == 1 && unlocks == 1 && try_rc == 0 && contender_rc == 0;
#elif EXPECT_CLEAN == 0
  bool expected = joined_canceled && cleanups == 0 && unlocks == 0 && try_rc == EBUSY && contender_rc == ETIMEDOUT;
#else
  bool clean = cleanups == 1 && unlocks == 1 && try_rc == 0 && contender_rc == 0;
  bool retained_lock = cleanups == 0 && unlocks == 0 && try_rc == EBUSY && contender_rc == ETIMEDOUT;
  bool expected = joined_canceled && (clean || retained_lock);
#endif
  std::printf("event=assert expected=%d\n", expected ? 1 : 0);
  return expected ? 0 : 66;
}
