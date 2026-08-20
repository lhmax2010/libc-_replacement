#ifdef USE_LIBCXX
#  include <cxxabi.h>
#else
#  include <bits/cxxabi_forced.h>
#endif

namespace abi = __cxxabiv1;

struct Derived final : abi::__forced_unwind {
  void __pure_dummy() override {}
};

Derived derived_instance;
