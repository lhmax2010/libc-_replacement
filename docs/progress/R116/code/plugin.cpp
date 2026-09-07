#include <execinfo.h>
extern "C" __attribute__((noinline)) int plugin_trace() { void* stack[32];return backtrace(stack,32); }
extern "C" __attribute__((noinline)) void plugin_throw(int* cleaned) {
  struct G { int* p; ~G(){++*p;} } g{cleaned}; throw 42;
}
