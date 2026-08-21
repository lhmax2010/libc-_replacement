#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <future>
#include <pthread.h>
#include <string>
#include <sys/syscall.h>
#include <unistd.h>

namespace {
std::atomic<pid_t> worker_tid{0};
std::atomic<unsigned long> worker_thread{0};

pid_t current_tid() {
  return static_cast<pid_t>(syscall(SYS_gettid));
}
} // namespace

void cancellable_body() {
    worker_tid.store(current_tid(), std::memory_order_release);
    worker_thread.store(static_cast<unsigned long>(pthread_self()), std::memory_order_release);
    for (;;)
      pthread_testcancel();
}

void* packaged_worker(void* opaque) {
  (*static_cast<std::packaged_task<void()>*>(opaque))();
  return nullptr;
}

template <class Future>
int observe_after_cancel(const char* mode, Future& future, pthread_t thread) {

  for (int i = 0; i < 500 && worker_tid.load(std::memory_order_acquire) == 0; ++i)
    usleep(10000);

  const pid_t tid = worker_tid.load(std::memory_order_acquire);
  const int cancel_rc = pthread_cancel(thread);
  usleep(500000);

  char task_path[128];
  std::snprintf(task_path, sizeof(task_path), "/proc/self/task/%ld", static_cast<long>(tid));
  const bool worker_present = access(task_path, F_OK) == 0;
  const std::future_status status = future.wait_for(std::chrono::milliseconds(0));

  std::fprintf(stderr,
               "MODE=%s CANCEL_RC=%d WORKER_TID=%ld WORKER_TASK_PRESENT=%d FUTURE_STATUS=%s\n",
               mode,
               cancel_rc,
               static_cast<long>(tid),
               worker_present ? 1 : 0,
               status == std::future_status::ready ? "ready"
                                                   : status == std::future_status::deferred ? "deferred" : "timeout");
  if (status == std::future_status::ready) {
    try {
      future.get();
      std::fprintf(stderr, "MODE=%s FUTURE_GET=value\n", mode);
    } catch (const std::future_error& error) {
      std::fprintf(stderr, "MODE=%s FUTURE_GET=future_error CODE=%d MESSAGE=%s\n", mode,
                   error.code().value(), error.what());
    } catch (const std::exception& error) {
      std::fprintf(stderr, "MODE=%s FUTURE_GET=std_exception MESSAGE=%s\n", mode, error.what());
    } catch (...) {
      std::fprintf(stderr, "MODE=%s FUTURE_GET=unknown_exception\n", mode);
    }
  }
  std::fflush(stderr);
  _Exit(0);
}

int main(int argc, char** argv) {
  if (argc != 2)
    return 64;
  if (std::string(argv[1]) == "async") {
    auto future = std::async(std::launch::async, cancellable_body);
    for (int i = 0; i < 500 && worker_thread.load(std::memory_order_acquire) == 0; ++i)
      usleep(10000);
    return observe_after_cancel("async", future,
                                static_cast<pthread_t>(worker_thread.load(std::memory_order_acquire)));
  }
  if (std::string(argv[1]) == "packaged_task") {
    std::packaged_task<void()> task(cancellable_body);
    auto future = task.get_future();
    pthread_t thread;
    const int create_rc = pthread_create(&thread, nullptr, packaged_worker, &task);
    if (create_rc != 0)
      return 65;
    return observe_after_cancel("packaged_task", future, thread);
  }
  return 66;
}
