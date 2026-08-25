#include <atomic>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <future>
#include <iostream>
#include <pthread.h>
#include <sstream>
#include <streambuf>
#include <thread>
#include <unwind.h>
#include <unistd.h>

pthread_t m2_worker_thread{};

namespace {

std::atomic<int> ready{0};
std::atomic<int> invocations{0};
std::atomic<int> m2_window_entered{0};

void terminate_marker() noexcept {
  const char msg[] = "TERMINATE_HANDLER=ENTERED\n";
  (void)!write(2, msg, sizeof(msg) - 1);
  _Exit(86);
}

struct ForeignException {
  _Unwind_Exception unwind;
};

void foreign_cleanup(_Unwind_Reason_Code reason, _Unwind_Exception* exc) {
  std::cerr << "FOREIGN_CLEANUP_REASON=" << static_cast<int>(reason) << '\n';
  delete reinterpret_cast<ForeignException*>(exc);
}

[[noreturn]] void raise_foreign() {
  auto* f = new ForeignException{};
  std::memcpy(&f->unwind.exception_class, "R59FORGN", 8);
  f->unwind.exception_cleanup = foreign_cleanup;
  const auto rc = _Unwind_RaiseException(&f->unwind);
  std::cerr << "UNWIND_RAISE_RETURN=" << static_cast<int>(rc) << '\n';
  _Unwind_DeleteException(&f->unwind);
  std::abort();
}

class ForeignOutputBuf : public std::streambuf {
protected:
  int_type overflow(int_type) override { raise_foreign(); }
};

int run_f1() {
  std::set_terminate(terminate_marker);
  ForeignOutputBuf buf;
  std::ostream os(&buf);
  os.exceptions(std::ios::badbit);
  try {
    os.put('x');
    std::cout << "F1_UNEXPECTED_RETURN\n";
    return 3;
  } catch (...) {
    std::cout << "F1_TOP_CATCH=ENTERED STATE_BAD=" << os.bad() << '\n';
    return os.bad() ? 0 : 4;
  }
}

class CancelInputBuf : public std::streambuf {
protected:
  int_type underflow() override {
    ready.store(1, std::memory_order_release);
    for (;;) {
      pthread_testcancel();
      usleep(1000);
    }
  }
};

struct F2Context {
  std::istream* input;
  std::streambuf* output;
};

void* f2_worker(void* opaque) {
  auto* ctx = static_cast<F2Context*>(opaque);
  ctx->input->get(*ctx->output, '\n');
  return reinterpret_cast<void*>(0x52);
}

int run_f2() {
  std::set_terminate(terminate_marker);
  CancelInputBuf source;
  std::stringbuf sink;
  std::istream input(&source);
  F2Context ctx{&input, &sink};
  pthread_t thread{};
  const int create_rc = pthread_create(&thread, nullptr, f2_worker, &ctx);
  while (!ready.load(std::memory_order_acquire))
    usleep(1000);
  const int cancel_rc = pthread_cancel(thread);
  void* result = nullptr;
  const int join_rc = pthread_join(thread, &result);
  std::cout << "F2_CREATE_RC=" << create_rc << " CANCEL_RC=" << cancel_rc << " JOIN_RC=" << join_rc
            << " JOIN_CANCELED=" << (result == PTHREAD_CANCELED) << " RDSTATE=" << input.rdstate()
            << " BAD=" << input.bad() << " FAIL=" << input.fail() << '\n';
  return (create_rc == 0 && cancel_rc == 0 && join_rc == 0 && result == PTHREAD_CANCELED && input.bad()) ? 0 : 5;
}

std::shared_future<int>* deferred_future = nullptr;

void* wait_deferred(void*) {
  deferred_future->wait();
  return reinterpret_cast<void*>(0x59);
}

int run_m1() {
  std::set_terminate(terminate_marker);
  auto future = std::async(std::launch::deferred, [] {
                  const int n = ++invocations;
                  std::cout << "M1_CALLABLE_INVOCATION=" << n << '\n';
                  if (n == 1) {
                    ready.store(1, std::memory_order_release);
                    for (;;) {
                      pthread_testcancel();
                      usleep(1000);
                    }
                  }
                  return 42;
                }).share();
  deferred_future = &future;
  pthread_t first{};
  pthread_create(&first, nullptr, wait_deferred, nullptr);
  while (!ready.load(std::memory_order_acquire))
    usleep(1000);
  const int cancel_rc = pthread_cancel(first);
  void* first_result = nullptr;
  const int first_join = pthread_join(first, &first_result);
  std::cout << "M1_FIRST_CANCEL_RC=" << cancel_rc << " FIRST_JOIN_RC=" << first_join
            << " FIRST_CANCELED=" << (first_result == PTHREAD_CANCELED) << '\n';

  std::atomic<int> second_done{0};
  std::thread second([&] {
    future.wait();
    second_done.store(1, std::memory_order_release);
  });
  for (int i = 0; i != 2000 && !second_done.load(std::memory_order_acquire); ++i)
    usleep(1000);
  if (!second_done.load(std::memory_order_acquire)) {
    std::cout << "M1_SECOND_WAIT=TIMEOUT INVOCATIONS=" << invocations.load() << '\n' << std::flush;
    second.detach();
    _Exit(6);
  }
  second.join();
  const int value = future.get();
  std::cout << "M1_SECOND_WAIT=RETURNED INVOCATIONS=" << invocations.load() << " VALUE=" << value << '\n';
  return value == 42 ? 0 : 7;
}

struct AsyncWindowResult {
  bool hold_worker = true;
  AsyncWindowResult() = default;
  AsyncWindowResult(const AsyncWindowResult&) = delete;
  AsyncWindowResult& operator=(const AsyncWindowResult&) = delete;
  AsyncWindowResult(AsyncWindowResult&&) noexcept : hold_worker(false) {}
  ~AsyncWindowResult() noexcept(false) {
    if (!hold_worker)
      return;
    m2_window_entered.store(1, std::memory_order_release);
    for (;;)
      asm volatile("" ::: "memory");
  }
};

int run_m2() {
  std::set_terminate(terminate_marker);
  auto f = std::async(std::launch::async, [] {
    m2_worker_thread = pthread_self();
    int old_type = 0;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old_type);
    return AsyncWindowResult{};
  });
  while (!m2_window_entered.load(std::memory_order_acquire))
    usleep(1000);
  std::cout << "M2_WINDOW=AFTER_SET_VALUE_BEFORE_TRY_EXIT\n" << std::flush;
  const int cancel_rc = pthread_cancel(m2_worker_thread);
  std::cout << "M2_CANCEL_RC=" << cancel_rc << '\n' << std::flush;
  sleep(1);
  std::cout << "M2_PROCESS_SURVIVED_CANCEL_PROPAGATION=1\n" << std::flush;
  try {
    (void)f.get();
    std::cout << "M2_FUTURE_GET=VALUE\n" << std::flush;
  } catch (const std::exception& e) {
    std::cout << "M2_FUTURE_GET=STD_EXCEPTION WHAT=" << e.what() << '\n' << std::flush;
  } catch (...) {
    std::cout << "M2_FUTURE_GET=NON_STD_EXCEPTION\n" << std::flush;
  }
  return 0;
}

} // namespace

int main(int argc, char** argv) {
  if (argc != 2)
    return 64;
  if (std::strcmp(argv[1], "f1") == 0)
    return run_f1();
  if (std::strcmp(argv[1], "f2") == 0)
    return run_f2();
  if (std::strcmp(argv[1], "m1") == 0)
    return run_m1();
  if (std::strcmp(argv[1], "m2") == 0)
    return run_m2();
  return 65;
}
