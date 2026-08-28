#include <exception>
#include <stdexcept>
#include <cstdio>
#include <cstring>

namespace std { [[noreturn]] void __throw_bad_alloc(); }

struct DuringUnwind {
  ~DuringUnwind() {
    std::printf("during_unwind_bool=%d\n", std::uncaught_exception() ? 1 : 0);
    std::printf("during_unwind_count=%d\n", std::uncaught_exceptions());
  }
};

static void throw_with_guard() {
  DuringUnwind guard;
  throw 7;
}

int main() {
  std::logic_error l1("logic"), l2(l1), l3("other");
  l3 = l2;
  std::runtime_error r1("runtime"), r2(r1), r3("other");
  r3 = r2;
  std::printf("logic_copy_assign=%d\n", std::strcmp(l2.what(), "logic") == 0 && std::strcmp(l3.what(), "logic") == 0);
  std::printf("runtime_copy_assign=%d\n", std::strcmp(r2.what(), "runtime") == 0 && std::strcmp(r3.what(), "runtime") == 0);
  std::printf("current_outside=%d\n", std::current_exception() ? 1 : 0);
  try {
    throw 9;
  } catch (...) {
    std::printf("current_inside=%d\n", std::current_exception() ? 1 : 0);
  }
  try {
    throw_with_guard();
  } catch (int value) {
    std::printf("caught_value=%d\n", value);
  }
  {
    std::nested_exception nested;
    std::printf("nested_empty=%d\n", nested.nested_ptr() ? 0 : 1);
  }
  try {
    std::__throw_bad_alloc();
  } catch (const std::bad_alloc&) {
    std::puts("bad_alloc_caught=1");
  }
  return 0;
}
