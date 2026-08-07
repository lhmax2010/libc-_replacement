#include <cstdio>

extern "C" void dso_throw_rethrow();
extern "C" int dso_destructor_total();

int main() {
  int caught_value = -1;
  try {
    dso_throw_rethrow();
  } catch (int value) {
    caught_value = value;
    std::printf("EVENT MAIN_CATCH_INT value=%d\n", value);
  } catch (...) {
    std::printf("EVENT MAIN_WRONG_CATCH\n");
    return 30;
  }

  const int destructor_total = dso_destructor_total();
  std::printf("RESULT caught=%d destructor_total=%d\n", caught_value,
              destructor_total);
  if (caught_value != 73 || destructor_total != 3) {
    return 31;
  }
  return 0;
}
