#include <pthread.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <stdexcept>

#if defined(_LIBCPP_VERSION)
#  include <cxxabi.h>
#else
#  include <bits/cxxabi_forced.h>
#endif

namespace {

struct Cleanup {
  const char* label;
  ~Cleanup() { std::fprintf(stderr, "cleanup:%s\n", label); }
};

void* cancel_worker(void*) {
  try {
    Cleanup cleanup{"cancel"};
    std::fprintf(stderr, "cancel:ready\n");
    for (;;)
      ::sleep(1);
  } catch (const __cxxabiv1::__forced_unwind&) {
    std::fprintf(stderr, "cancel:forced-handler\n");
    throw;
  } catch (...) {
    std::fprintf(stderr, "cancel:catch-all\n");
  }
  return reinterpret_cast<void*>(0x11);
}

void* exit_worker(void*) {
  try {
    Cleanup cleanup{"exit"};
    std::fprintf(stderr, "exit:before-pthread-exit\n");
    ::pthread_exit(reinterpret_cast<void*>(0x2a));
  } catch (const __cxxabiv1::__forced_unwind&) {
    std::fprintf(stderr, "exit:forced-handler\n");
    throw;
  } catch (...) {
    std::fprintf(stderr, "exit:catch-all\n");
  }
  return reinterpret_cast<void*>(0x12);
}

int run_cancel() {
  pthread_t thread;
  if (::pthread_create(&thread, nullptr, cancel_worker, nullptr) != 0)
    return 20;
  ::sleep(1);
  if (::pthread_cancel(thread) != 0)
    return 21;
  void* result = nullptr;
  if (::pthread_join(thread, &result) != 0)
    return 22;
  std::fprintf(stderr, "cancel:join=%s\n",
               result == PTHREAD_CANCELED ? "PTHREAD_CANCELED" : "OTHER");
  return result == PTHREAD_CANCELED ? 0 : 23;
}

int run_real() {
  try {
    throw std::runtime_error("r33-real");
  } catch (const __cxxabiv1::__forced_unwind&) {
    std::fprintf(stderr, "real:forced-handler:MISROUTED\n");
    return 30;
  } catch (const std::runtime_error& error) {
    std::fprintf(stderr, "real:runtime_error:%s\n", error.what());
    return 0;
  } catch (...) {
    std::fprintf(stderr, "real:catch-all\n");
    return 31;
  }
}

int run_exit() {
  pthread_t thread;
  if (::pthread_create(&thread, nullptr, exit_worker, nullptr) != 0)
    return 40;
  void* result = nullptr;
  if (::pthread_join(thread, &result) != 0)
    return 41;
  std::fprintf(stderr, "exit:join=%p\n", result);
  return result == reinterpret_cast<void*>(0x2a) ? 0 : 42;
}

} // namespace

int main(int argc, char** argv) {
  if (argc != 2)
    return 64;
  if (__builtin_strcmp(argv[1], "cancel") == 0)
    return run_cancel();
  if (__builtin_strcmp(argv[1], "real") == 0)
    return run_real();
  if (__builtin_strcmp(argv[1], "exit") == 0)
    return run_exit();
  return 65;
}
