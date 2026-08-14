#include <pthread.h>
#include <unwind.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(R36_LIBCXXABI)
#  include <cxxabi.h>
#else
#  include <bits/cxxabi_forced.h>
#endif

namespace {

struct Cleanup {
  const char* label;
  ~Cleanup() { fprintf(stderr, "cleanup:%s\n", label); }
};

struct RealException {
  int marker;
};

void* cancel_worker(void*) {
  try {
    Cleanup cleanup{"cancel"};
    fprintf(stderr, "cancel:ready\n");
    for (;;)
      sleep(1);
  } catch (const __cxxabiv1::__forced_unwind&) {
    fprintf(stderr, "cancel:forced-handler\n");
    throw;
  } catch (...) {
    fprintf(stderr, "cancel:catch-all\n");
  }
  return reinterpret_cast<void*>(0x11);
}

void* cancel_no_catch_worker(void*) {
  Cleanup cleanup{"cancel-no-catch"};
  fprintf(stderr, "cancel-no-catch:ready\n");
  for (;;)
    sleep(1);
}

void* exit_worker(void*) {
  try {
    Cleanup cleanup{"exit"};
    fprintf(stderr, "exit:before-pthread-exit\n");
    pthread_exit(reinterpret_cast<void*>(0x2a));
  } catch (const __cxxabiv1::__forced_unwind&) {
    fprintf(stderr, "exit:forced-handler\n");
    throw;
  } catch (...) {
    fprintf(stderr, "exit:catch-all\n");
  }
  return reinterpret_cast<void*>(0x12);
}

void* exit_no_catch_worker(void*) {
  Cleanup cleanup{"exit-no-catch"};
  fprintf(stderr, "exit-no-catch:before-pthread-exit\n");
  pthread_exit(reinterpret_cast<void*>(0x2a));
}

int run_cancel(void* (*worker)(void*), const char* label) {
  pthread_t thread;
  if (pthread_create(&thread, nullptr, worker, nullptr) != 0)
    return 20;
  sleep(1);
  if (pthread_cancel(thread) != 0)
    return 21;
  void* result = nullptr;
  if (pthread_join(thread, &result) != 0)
    return 22;
  fprintf(stderr, "%s:join=%s\n", label,
          result == PTHREAD_CANCELED ? "PTHREAD_CANCELED" : "OTHER");
  return result == PTHREAD_CANCELED ? 0 : 23;
}

int run_exit(void* (*worker)(void*), const char* label) {
  pthread_t thread;
  if (pthread_create(&thread, nullptr, worker, nullptr) != 0)
    return 40;
  void* result = nullptr;
  if (pthread_join(thread, &result) != 0)
    return 41;
  fprintf(stderr, "%s:join=%p\n", label, result);
  return result == reinterpret_cast<void*>(0x2a) ? 0 : 42;
}

int run_real() {
  try {
    Cleanup cleanup{"real"};
    fprintf(stderr, "real:before-throw\n");
    throw RealException{34};
  } catch (const __cxxabiv1::__forced_unwind&) {
    fprintf(stderr, "real:forced-handler:MISROUTED\n");
    return 30;
  } catch (const RealException& error) {
    fprintf(stderr, "real:typed-exception:%d\n", error.marker);
    return error.marker == 34 ? 0 : 32;
  } catch (...) {
    fprintf(stderr, "real:catch-all\n");
    return 31;
  }
}

struct ForeignException {
  _Unwind_Exception unwind;
};

ForeignException* active_foreign = nullptr;
int foreign_cleanup_count = 0;

void foreign_cleanup(_Unwind_Reason_Code reason, _Unwind_Exception* exception) {
  ++foreign_cleanup_count;
  fprintf(stderr, "foreign:cleanup:reason=%d:count=%d\n",
          static_cast<int>(reason), foreign_cleanup_count);
  free(reinterpret_cast<ForeignException*>(exception));
  active_foreign = nullptr;
}

unsigned long long foreign_forced_state(const _Unwind_Exception& exception) {
#if defined(__ARM_EABI__)
  return static_cast<unsigned long long>(exception.unwinder_cache.reserved1);
#else
  return static_cast<unsigned long long>(exception.private_1);
#endif
}

int run_foreign() {
  active_foreign = static_cast<ForeignException*>(
      calloc(1, sizeof(ForeignException)));
  if (active_foreign == nullptr)
    return 50;
  active_foreign->unwind.exception_class =
      static_cast<_Unwind_Exception_Class>(0x52333442464f5245ULL);
  active_foreign->unwind.exception_cleanup = foreign_cleanup;

  try {
    try {
      Cleanup cleanup{"foreign-inner"};
      fprintf(stderr, "foreign:raise\n");
      const _Unwind_Reason_Code reason =
          _Unwind_RaiseException(&active_foreign->unwind);
      fprintf(stderr, "foreign:raise-returned:%d\n", static_cast<int>(reason));
      _Unwind_DeleteException(&active_foreign->unwind);
      return 51;
    } catch (...) {
      fprintf(stderr, "foreign:inner-catch:forced-state=%llu:rethrow\n",
              foreign_forced_state(active_foreign->unwind));
      throw;
    }
  } catch (...) {
    fprintf(stderr, "foreign:outer-catch\n");
  }

  fprintf(stderr, "foreign:after-catch:cleanup-count=%d\n",
          foreign_cleanup_count);
  return foreign_cleanup_count == 1 ? 0 : 52;
}

} // namespace

int main(int argc, char** argv) {
  if (argc != 2)
    return 64;
  if (strcmp(argv[1], "cancel") == 0)
    return run_cancel(cancel_worker, "cancel");
  if (strcmp(argv[1], "exit") == 0)
    return run_exit(exit_worker, "exit");
  if (strcmp(argv[1], "real") == 0)
    return run_real();
  if (strcmp(argv[1], "foreign") == 0)
    return run_foreign();
  if (strcmp(argv[1], "cancel-no-catch") == 0)
    return run_cancel(cancel_no_catch_worker, "cancel-no-catch");
  if (strcmp(argv[1], "exit-no-catch") == 0)
    return run_exit(exit_no_catch_worker, "exit-no-catch");
  return 65;
}
