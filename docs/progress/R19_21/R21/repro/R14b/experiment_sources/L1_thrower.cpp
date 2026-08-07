#include <cstdio>

namespace {

int dso_outer_destructors = 0;
int dso_inner_destructors = 0;
int dso_rethrow_destructors = 0;

struct Guard {
  const char* name;
  int* counter;

  Guard(const char* n, int* c) : name(n), counter(c) {
    std::printf("EVENT DSO_CONSTRUCT %s\n", name);
  }

  ~Guard() {
    ++*counter;
    std::printf("EVENT DSO_DESTRUCT %s count=%d\n", name, *counter);
  }
};

}  // namespace

extern "C" __attribute__((visibility("default"))) void dso_throw_rethrow() {
  Guard outer("outer", &dso_outer_destructors);
  try {
    Guard inner("inner", &dso_inner_destructors);
    std::printf("EVENT DSO_THROW value=73\n");
    throw 73;
  } catch (...) {
    std::printf("EVENT DSO_CATCH_ANY before_rethrow\n");
    Guard rethrow_guard("rethrow_guard", &dso_rethrow_destructors);
    std::printf("EVENT DSO_RETHROW\n");
    throw;
  }
}

extern "C" __attribute__((visibility("default"))) int
dso_destructor_total() {
  return dso_outer_destructors + dso_inner_destructors +
         dso_rethrow_destructors;
}
