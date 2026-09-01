#ifndef PROBE_NOEXCEPT
#  define PROBE_NOEXCEPT
#endif

namespace probe {

void d01(int&) PROBE_NOEXCEPT {}

struct condition_like {
  void d02(long) PROBE_NOEXCEPT;
};

void condition_like::d02(long) PROBE_NOEXCEPT {}

template <class Clock>
__attribute__((noinline)) void d04(Clock) PROBE_NOEXCEPT {}

template void d04<int>(int) PROBE_NOEXCEPT;

void pointer_target() PROBE_NOEXCEPT {}

template <class Pointer>
__attribute__((noinline)) void consume_pointer_type(Pointer) {}

void instantiate_pointer_type() { consume_pointer_type(&pointer_target); }

} // namespace probe
