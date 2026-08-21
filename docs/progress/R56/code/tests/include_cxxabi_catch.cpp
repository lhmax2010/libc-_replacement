#include <cxxabi.h>

int probe_complete_definition() {
  try {
    throw 7;
  } catch (const __cxxabiv1::__forced_unwind&) {
    throw;
  } catch (...) {
    return 0;
  }
}
