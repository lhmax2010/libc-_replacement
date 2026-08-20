#ifdef USE_LIBCXX
#  include <cxxabi.h>
#else
#  include <bits/cxxabi_forced.h>
#endif

namespace abi = __cxxabiv1;

auto* forced_unwind_instance = new abi::__forced_unwind;
