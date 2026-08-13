#include <pthread.h>
#include <sched.h>
#include <unistd.h>

#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <stdexcept>
#include <thread>
#include <typeinfo>

#include <cxxabi.h>

static std::atomic<int> started{0};
static std::atomic<int> cleanup_count{0};
static const char* selected_case = nullptr;

static void event(const char* value) {
  std::fprintf(stderr, "EVENT %s\n", value);
  std::fflush(stderr);
}

static void report_exception_state(const char* prefix) {
  const std::type_info* ti = __cxxabiv1::__cxa_current_exception_type();
  const std::exception_ptr ep = std::current_exception();
  std::fprintf(stderr, "EVENT %s cxa_type=%s current_exception=%d\n", prefix,
               ti ? ti->name() : "NULL", ep ? 1 : 0);
}

struct Cleanup {
  ~Cleanup() {
    ++cleanup_count;
    event("cleanup_destructor");
  }
};

static void cancel_point() {
  started.store(1);
  for (;;) {
    pthread_testcancel();
    sched_yield();
  }
}

static void* cancel_worker(void*) {
  Cleanup cleanup;
  if (std::strcmp(selected_case, "no_catch_cancel") == 0) {
    cancel_point();
  } else if (std::strcmp(selected_case, "baseline_cancel") == 0) {
    try {
      cancel_point();
    } catch (...) {
      event("baseline_catch_swallow");
    }
#if defined(__GLIBCXX__)
  } else if (std::strcmp(selected_case, "forced_guard_cancel") == 0) {
    try {
      cancel_point();
    } catch (const __cxxabiv1::__forced_unwind&) {
      event("forced_guard_rethrow");
      throw;
    } catch (...) {
      event("forced_guard_unexpected_catch_all");
    }
#endif
  } else if (std::strcmp(selected_case, "a_observe_cancel") == 0) {
    try {
      cancel_point();
    } catch (...) {
      report_exception_state("a_observe_forced");
      event("a_observe_swallow");
    }
  } else if (std::strcmp(selected_case, "a_detect_cancel") == 0) {
    try {
      cancel_point();
    } catch (...) {
      const std::type_info* ti = __cxxabiv1::__cxa_current_exception_type();
      const std::exception_ptr ep = std::current_exception();
      std::fprintf(stderr, "EVENT a_detect cxa_type=%s current_exception=%d\n",
                   ti ? ti->name() : "NULL", ep ? 1 : 0);
      if (ti == nullptr || !ep) {
        event("a_detect_foreign_rethrow");
        throw;
      }
      event("a_detect_treated_as_cpp_and_swallowed");
    }
  } else if (std::strcmp(selected_case, "b_cancel") == 0) {
    try {
      cancel_point();
    } catch (const std::exception&) {
      event("b_std_exception_catch");
    }
  } else if (std::strcmp(selected_case, "d_cancel") == 0) {
    try {
      cancel_point();
    } catch (...) {
      event("d_cleanup_then_rethrow");
      throw;
    }
  }
  event("worker_returned");
  return reinterpret_cast<void*>(0x1234);
}

static void* cancellation_disabled_worker(void*) {
  Cleanup cleanup;
  int old_state = -1;
  const int disable_rc = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_state);
  std::fprintf(stderr, "EVENT c_disable rc=%d old_state=%d\n", disable_rc, old_state);
  started.store(1);
  usleep(150000);
  event("c_work_completed_while_disabled");
  if (std::strcmp(selected_case, "c_restore_cancel") == 0) {
    event("c_before_restore");
    const int restore_rc = pthread_setcancelstate(old_state, nullptr);
    std::fprintf(stderr, "EVENT c_after_restore rc=%d\n", restore_rc);
    pthread_testcancel();
  }
  event("c_normal_return_with_cancel_disabled");
  return reinterpret_cast<void*>(0x5678);
}

static int run_cancel_case() {
  pthread_t thread{};
  void* result = nullptr;
  void* (*entry)(void*) =
      std::strncmp(selected_case, "c_", 2) == 0 ? cancellation_disabled_worker
                                                : cancel_worker;
  const int create_rc = pthread_create(&thread, nullptr, entry, nullptr);
  if (create_rc != 0) {
    std::fprintf(stderr, "RESULT create_rc=%d\n", create_rc);
    return 10;
  }
  while (started.load() == 0)
    usleep(1000);
  const int cancel_rc = pthread_cancel(thread);
  const int join_rc = pthread_join(thread, &result);
  const bool canceled = result == PTHREAD_CANCELED;
  std::fprintf(stderr,
               "RESULT cancel_rc=%d join_rc=%d canceled=%d result=%p cleanup=%d\n",
               cancel_rc, join_rc, canceled ? 1 : 0, result, cleanup_count.load());
  if (std::strcmp(selected_case, "c_no_restore_cancel") == 0)
    return cancel_rc == 0 && join_rc == 0 && !canceled && result == reinterpret_cast<void*>(0x5678) ? 0 : 21;
  return cancel_rc == 0 && join_rc == 0 && canceled && cleanup_count.load() == 1 ? 0 : 20;
}

static int run_real_case() {
  const bool throw_int = std::strstr(selected_case, "real_int") != nullptr;
  bool inner_handled = false;
  bool outer_observed = false;
  try {
    if (selected_case[0] == 'a') {
      try {
        if (throw_int)
          throw 42;
        throw std::runtime_error("r32-real-error");
      } catch (...) {
        report_exception_state("a_observe_real");
        const std::type_info* ti = __cxxabiv1::__cxa_current_exception_type();
        const std::exception_ptr ep = std::current_exception();
        if (ti == nullptr || !ep) {
          event("a_real_misclassified_foreign");
          throw;
        }
        event("a_real_handled");
        inner_handled = true;
      }
    } else if (selected_case[0] == 'b') {
      try {
        if (throw_int)
          throw 42;
        throw std::runtime_error("r32-real-error");
      } catch (const std::exception&) {
        event("b_caught_std_exception");
        inner_handled = true;
      } catch (...) {
        event("b_specific_catch_did_not_match");
        throw;
      }
    } else {
      try {
        if (throw_int)
          throw 42;
        throw std::runtime_error("r32-real-error");
      } catch (...) {
        event("d_real_cleanup_then_rethrow");
        throw;
      }
    }
  } catch (const std::runtime_error&) {
    event("outer_caught_runtime_error");
    outer_observed = true;
  } catch (int value) {
    std::fprintf(stderr, "EVENT outer_caught_int value=%d\n", value);
    outer_observed = true;
  }
  std::fprintf(stderr, "RESULT inner_handled=%d outer_observed=%d\n",
               inner_handled ? 1 : 0, outer_observed ? 1 : 0);
  if (selected_case[0] == 'a')
    return inner_handled && !outer_observed ? 0 : 30;
  if (selected_case[0] == 'b')
    return throw_int ? (!inner_handled && outer_observed ? 0 : 31)
                     : (inner_handled && !outer_observed ? 0 : 32);
  return !inner_handled && outer_observed ? 0 : 33;
}

int main(int argc, char** argv) {
  std::setvbuf(stderr, nullptr, _IONBF, 0);
  std::set_terminate([] {
    event("terminate_handler");
    std::abort();
  });
  if (argc != 2) {
    std::fprintf(stderr, "usage: %s CASE\n", argv[0]);
    return 64;
  }
  selected_case = argv[1];
#if defined(_LIBCPP_VERSION)
  event("stdlib=libc++");
#elif defined(__GLIBCXX__)
  event("stdlib=libstdc++");
#else
  event("stdlib=unknown");
#endif
  std::fprintf(stderr, "EVENT case=%s execution=HOST_NATIVE_X86_64_NO_QEMU\n",
               selected_case);
  if (std::strcmp(selected_case, "forced_guard_cancel") == 0) {
#if !defined(__GLIBCXX__)
    event("not_available_no_forced_unwind_type");
    return 77;
#endif
  }
  if (std::strstr(selected_case, "real_") != nullptr)
    return run_real_case();
  return run_cancel_case();
}
