#ifdef USE_LIBCXX
#  include <cxxabi.h>
#else
#  include <bits/cxxabi_forced.h>
#endif

namespace abi = __cxxabiv1;

void probe_by_value() {
  try {
    throw 1;
  } catch (abi::__forced_unwind) {
    throw;
  } catch (...) {
  }
}
