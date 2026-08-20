#ifdef USE_LIBCXX
#  include <cxxabi.h>
#else
#  include <bits/cxxabi_forced.h>
#endif

namespace abi = __cxxabiv1;

void delete_forced_unwind(abi::__forced_unwind* value) {
  delete value;
}
