#include <pthread.h>
#include <sched.h>
#include <unistd.h>

#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <thread>

#if defined(__GLIBCXX__)
#  include <cxxabi.h>
#endif

#ifndef CASE_KIND
#  error CASE_KIND must be defined
#endif

static std::atomic<int> started{0};
static std::atomic<int> destructor_count{0};
static std::atomic<int> forced_guard_count{0};
static std::atomic<int> catch_all_count{0};
static std::atomic<int> returned_count{0};

static void event(const char* value) {
  std::fprintf(stderr, "EVENT %s\n", value);
  std::fflush(stderr);
}

struct Cleanup {
  ~Cleanup() {
    ++destructor_count;
    event("destructor");
  }
};

static void cancel_loop() {
  int old_state = -1;
  int old_type = -1;
  const int state_rc = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old_state);
  const int type_rc = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &old_type);
  std::fprintf(stderr,
               "EVENT cancel_config state_rc=%d old_state=%d type_rc=%d old_type=%d\n",
               state_rc, old_state, type_rc, old_type);
  started.store(1);
  for (;;) {
    pthread_testcancel();
    sched_yield();
  }
}

static void run_case_body() {
  Cleanup cleanup;
#if CASE_KIND == 5
  cancel_loop();
#else
  try {
    cancel_loop();
#  if CASE_KIND == 2 && defined(__GLIBCXX__)
  } catch (const __cxxabiv1::__forced_unwind&) {
    ++forced_guard_count;
    event("forced_guard_rethrow");
    throw;
#  endif
  } catch (...) {
    ++catch_all_count;
    event("catch_all");
  }
  ++returned_count;
  event("worker_returned");
#endif
}

static void* pthread_worker(void*) {
  run_case_body();
  return nullptr;
}

static void* pthread_worker_noexcept(void*) noexcept {
  run_case_body();
  return nullptr;
}

static void install_terminate_observer() {
  std::set_terminate([] {
    event("terminate_handler");
    std::abort();
  });
}

static int report_result(int cancel_rc, int join_rc, bool canceled) {
  std::fprintf(stderr,
               "RESULT cancel_rc=%d join_rc=%d canceled=%d destructor=%d "
               "forced_guard=%d catch_all=%d returned=%d\n",
               cancel_rc, join_rc, canceled ? 1 : 0, destructor_count.load(),
               forced_guard_count.load(), catch_all_count.load(),
               returned_count.load());
  const bool ok = cancel_rc == 0 && join_rc == 0 && canceled &&
                  destructor_count.load() == 1 && catch_all_count.load() == 0 &&
                  returned_count.load() == 0;
  return ok ? 0 : 20;
}

int main() {
  std::setvbuf(stderr, nullptr, _IONBF, 0);
  install_terminate_observer();
#if defined(_LIBCPP_VERSION)
  event("stdlib=libc++");
#elif defined(__GLIBCXX__)
  event("stdlib=libstdc++");
#else
  event("stdlib=unknown");
#endif
  std::fprintf(stderr, "EVENT case_kind=%d\n", CASE_KIND);

#if CASE_KIND == 2 && !defined(__GLIBCXX__)
  event("not_available_no_forced_unwind_type");
  return 77;
#elif CASE_KIND == 4
  std::thread thread([] { run_case_body(); });
  while (started.load() == 0)
    usleep(1000);
  const int cancel_rc = pthread_cancel(thread.native_handle());
  thread.join();
  const bool canceled = destructor_count.load() == 1 &&
                        catch_all_count.load() == 0 && returned_count.load() == 0;
  return report_result(cancel_rc, 0, canceled);
#else
  pthread_t thread{};
  void* result = nullptr;
#  if CASE_KIND == 3
  void* (*entry)(void*) = pthread_worker_noexcept;
#  else
  void* (*entry)(void*) = pthread_worker;
#  endif
  const int create_rc = pthread_create(&thread, nullptr, entry, nullptr);
  if (create_rc != 0) {
    std::fprintf(stderr, "RESULT create_rc=%d\n", create_rc);
    return 10;
  }
  while (started.load() == 0)
    usleep(1000);
  const int cancel_rc = pthread_cancel(thread);
  const int join_rc = pthread_join(thread, &result);
  return report_result(cancel_rc, join_rc, result == PTHREAD_CANCELED);
#endif
}
