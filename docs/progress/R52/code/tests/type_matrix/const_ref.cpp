#ifdef USE_LIBCXX
#  include <cxxabi.h>
#else
#  include <bits/cxxabi_forced.h>
#endif

namespace abi = __cxxabiv1;

void probe_const_ref() {
  try {
    throw 1;
  } catch (const abi::__forced_unwind&) {
    throw;
  } catch (...) {
  }
}
