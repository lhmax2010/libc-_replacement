#include <condition_variable>
#include <limits>
#include <ratio>
#include <thread>
#include <type_traits>
#include <unistd.h>
#include <__chrono/duration.h>
#include <__chrono/system_clock.h>
#include <__chrono/time_point.h>
#include <__system_error/throw_system_error.h>

_LIBCPP_PUSH_MACROS
#include <__undef_macros>

_LIBCPP_BEGIN_NAMESPACE_STD

static_assert(is_standard_layout<condition_variable>::value, "condition_variable must be standard-layout");
static_assert(sizeof(condition_variable) == sizeof(__libcpp_condvar_t), "native condition variable must be sole state");

void condition_variable::notify_one() noexcept { __libcpp_condvar_signal(&__cv_); }

void condition_variable::notify_all() noexcept { __libcpp_condvar_broadcast(&__cv_); }

static void __r113_wait_body(condition_variable* self, unique_lock<mutex>& lk) {
  if (!lk.owns_lock())
    std::__throw_system_error(EPERM, "condition_variable::wait: mutex not locked");
  auto* native = reinterpret_cast<__libcpp_condvar_t*>(self);
  int ec = __libcpp_condvar_wait(native, lk.mutex()->native_handle());
  if (ec)
    std::__throw_system_error(ec, "condition_variable wait failed");
}

extern "C" __attribute__((visibility("default"))) void
__r113_wait_old(condition_variable* self, unique_lock<mutex>* lk) noexcept {
  constexpr char marker[] = "wrapper=old\n";
  (void)!write(STDOUT_FILENO, marker, sizeof(marker) - 1);
  __r113_wait_body(self, *lk);
}

extern "C" __attribute__((visibility("default"))) void
__r113_wait_new(condition_variable* self, unique_lock<mutex>* lk) {
  constexpr char marker[] = "wrapper=new\n";
  (void)!write(STDOUT_FILENO, marker, sizeof(marker) - 1);
  __r113_wait_body(self, *lk);
}

asm(".symver __r113_wait_old,_ZNSt3__118condition_variable4waitERNS_11unique_lockINS_5mutexEEE@LLVM_22");
asm(".symver __r113_wait_new,_ZNSt3__118condition_variable4waitERNS_11unique_lockINS_5mutexEEE@@LLVM_23");

void condition_variable::__do_timed_wait(unique_lock<mutex>& lk,
                                         chrono::time_point<chrono::system_clock, chrono::nanoseconds> tp) noexcept {
  using namespace chrono;
  if (!lk.owns_lock())
    std::__throw_system_error(EPERM, "condition_variable::timed wait: mutex not locked");
  nanoseconds d = tp.time_since_epoch();
  if (d > nanoseconds(0x59682F000000E941))
    d = nanoseconds(0x59682F000000E941);
  __libcpp_timespec_t ts;
  seconds s = duration_cast<seconds>(d);
  typedef decltype(ts.tv_sec) ts_sec;
  constexpr ts_sec ts_sec_max = numeric_limits<ts_sec>::max();
  if (s.count() < ts_sec_max) {
    ts.tv_sec = static_cast<ts_sec>(s.count());
    ts.tv_nsec = static_cast<decltype(ts.tv_nsec)>((d - s).count());
  } else {
    ts.tv_sec = ts_sec_max;
    ts.tv_nsec = giga::num - 1;
  }
  int ec = __libcpp_condvar_timedwait(&__cv_, lk.mutex()->native_handle(), &ts);
  if (ec != 0 && ec != ETIMEDOUT)
    std::__throw_system_error(ec, "condition_variable timed_wait failed");
}

void notify_all_at_thread_exit(condition_variable& cond, unique_lock<mutex> lk) {
  auto& tl_ptr = __thread_local_data();
  if (tl_ptr.get() == nullptr)
    tl_ptr.set_pointer(new __thread_struct);
  __thread_local_data()->notify_all_at_thread_exit(&cond, lk.release());
}

_LIBCPP_END_NAMESPACE_STD

_LIBCPP_POP_MACROS
