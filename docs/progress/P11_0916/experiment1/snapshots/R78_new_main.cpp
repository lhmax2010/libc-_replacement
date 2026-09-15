#include "boundary_error.hpp"

#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>
#include <link.h>
#include <pthread.h>
#include <string>
#include <unistd.h>

namespace {
using OldNormal = int (*)(const char*, char*, unsigned);
using OldThrow = void (*)();
using OldCancelWait = void (*)();
using OldWaitReady = int (*)();
using OldGuardValue = int (*)();

std::atomic<int> new_guard_count{0};

struct NewGuard {
  ~NewGuard() { new_guard_count.fetch_add(1, std::memory_order_relaxed); }
};

struct WorkerArgs {
  OldCancelWait wait;
};

void* worker(void* opaque) {
  NewGuard guard;
  static_cast<WorkerArgs*>(opaque)->wait();
  return reinterpret_cast<void*>(0xBAD);
}

int library_callback(dl_phdr_info* info, size_t, void*) {
  if (info->dlpi_name &&
      (std::strstr(info->dlpi_name, "libc++.so") ||
       std::strstr(info->dlpi_name, "libc++abi.so") ||
       std::strstr(info->dlpi_name, "libstdc++.so")))
    std::printf("LOADED=%s\n", info->dlpi_name);
  return 0;
}

template <class T>
T symbol(void* handle, const char* name) {
  dlerror();
  void* value = dlsym(handle, name);
  const char* error = dlerror();
  if (error) {
    std::fprintf(stderr, "DLSYM_ERROR name=%s error=%s\n", name, error);
    std::exit(20);
  }
  return reinterpret_cast<T>(value);
}
}  // namespace

int main(int argc, char** argv) {
  if (argc != 3) {
    std::fprintf(stderr, "usage: %s OLD_PLUGIN normal|exception|cancel\n", argv[0]);
    return 2;
  }
  void* handle = dlopen(argv[1], RTLD_NOW | RTLD_GLOBAL);
  if (!handle) {
    std::fprintf(stderr, "DLOPEN_ERROR=%s\n", dlerror());
    return 3;
  }
  dl_iterate_phdr(library_callback, nullptr);
  OldGuardValue old_guard_value = symbol<OldGuardValue>(handle, "old_guard_value");

  if (std::strcmp(argv[2], "normal") == 0) {
    char output[64] = {};
    int length = symbol<OldNormal>(handle, "old_normal")("ping", output, sizeof(output));
    bool ok = length == 8 && std::string(output) == "old:ping" && old_guard_value() == 1;
    std::printf("SCENARIO=normal LENGTH=%d OUTPUT=%s OLD_GUARD=%d ASSERT=%s\n",
                length, output, old_guard_value(), ok ? "PASS" : "FAIL");
    return ok ? 0 : 10;
  }

  if (std::strcmp(argv[2], "exception") == 0) {
    int caught = 0;
    try {
      symbol<OldThrow>(handle, "old_throw")();
    } catch (const BoundaryError& error) {
      caught = error.code;
    } catch (...) {
      std::fprintf(stderr, "WRONG_EXCEPTION_TYPE\n");
      return 12;
    }
    bool ok = caught == 77 && old_guard_value() == 1;
    std::printf("SCENARIO=exception CAUGHT=%d OLD_GUARD=%d ASSERT=%s\n",
                caught, old_guard_value(), ok ? "PASS" : "FAIL");
    return ok ? 0 : 11;
  }

  if (std::strcmp(argv[2], "cancel") == 0) {
    OldWaitReady ready = symbol<OldWaitReady>(handle, "old_wait_ready");
    WorkerArgs args{symbol<OldCancelWait>(handle, "old_cancel_wait")};
    pthread_t thread{};
    int create_rc = pthread_create(&thread, nullptr, worker, &args);
    if (create_rc != 0) {
      std::fprintf(stderr, "PTHREAD_CREATE=%d\n", create_rc);
      return 13;
    }
    int spins = 0;
    while (!ready() && spins++ < 5000)
      usleep(1000);
    if (!ready()) {
      std::fprintf(stderr, "WAIT_READY_TIMEOUT\n");
      return 14;
    }
    int cancel_rc = pthread_cancel(thread);
    void* result = nullptr;
    int join_rc = pthread_join(thread, &result);
    int new_guards = new_guard_count.load(std::memory_order_relaxed);
    int old_guards = old_guard_value();
    bool ok = cancel_rc == 0 && join_rc == 0 && result == PTHREAD_CANCELED &&
              new_guards == 1 && old_guards == 1;
    std::printf("SCENARIO=cancel CANCEL_RC=%d JOIN_RC=%d CANCELED=%d "
                "NEW_GUARD=%d OLD_GUARD=%d ASSERT=%s\n",
                cancel_rc, join_rc, result == PTHREAD_CANCELED,
                new_guards, old_guards, ok ? "PASS" : "FAIL");
    return ok ? 0 : 15;
  }

  std::fprintf(stderr, "UNKNOWN_SCENARIO=%s\n", argv[2]);
  return 4;
}
