#ifdef USE_LIBCXX
#  include <cxxabi.h>
#else
#  include <bits/cxxabi_forced.h>
#endif

namespace abi = __cxxabiv1;

[[maybe_unused]] constexpr auto forced_unwind_size = sizeof(abi::__forced_unwind);
