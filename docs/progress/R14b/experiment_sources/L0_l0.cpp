#include <cstdio>

namespace {

int outer_destructors = 0;
int inner_destructors = 0;
int rethrow_destructors = 0;

struct Guard {
  const char* name;
  int* counter;

  Guard(const char* n, int* c) : name(n), counter(c) {
    std::printf("EVENT CONSTRUCT %s\n", name);
  }

  ~Guard() {
    ++*counter;
    std::printf("EVENT DESTRUCT %s count=%d\n", name, *counter);
  }
};

void throw_rethrow() {
  Guard outer("outer", &outer_destructors);
  try {
    Guard inner("inner", &inner_destructors);
    std::printf("EVENT THROW value=42\n");
    throw 42;
  } catch (...) {
    std::printf("EVENT CATCH_ANY before_rethrow\n");
    Guard rethrow_guard("rethrow_guard", &rethrow_destructors);
    std::printf("EVENT RETHROW\n");
    throw;
  }
}

}  // namespace

int main() {
  int caught_value = -1;
  try {
    throw_rethrow();
  } catch (int value) {
    caught_value = value;
    std::printf("EVENT CATCH_INT value=%d\n", value);
  } catch (...) {
    std::printf("EVENT WRONG_CATCH\n");
    return 20;
  }

  std::printf("RESULT caught=%d outer=%d inner=%d rethrow=%d\n",
              caught_value, outer_destructors, inner_destructors,
              rethrow_destructors);
  if (caught_value != 42 || outer_destructors != 1 ||
      inner_destructors != 1 || rethrow_destructors != 1) {
    return 21;
  }
  return 0;
}
