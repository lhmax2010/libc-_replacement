#include <pthread.h>
#include <unwind.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

#if defined(_LIBCPP_VERSION)
#  include <cxxabi.h>
#else
#  include <bits/cxxabi_forced.h>
#endif

#if defined(__ARM_EABI__)
#  define private_1 unwinder_cache.reserved1
#endif
