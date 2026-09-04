#include <boost/thread/once.hpp>

#include <cstdio>
#include <dlfcn.h>
#include <fstream>
#include <string>
#include <type_traits>

namespace {
bool maps_contains(const char* token) {
  std::ifstream maps("/proc/self/maps");
  std::string line;
  while (std::getline(maps, line)) {
    if (line.find(token) != std::string::npos) return true;
  }
  return false;
}
}

int main() {
  Dl_info info{};
  const int address_ok = dladdr(
      reinterpret_cast<void*>(&boost::thread_detail::enter_once_region), &info);
  std::printf("runtime_maps=libcxx:%d,libstdcxx:%d,provider:%d dladdr=%d\n",
              maps_contains("libc++.so.1") ? 1 : 0,
              maps_contains("libstdc++.so.6") ? 1 : 0,
              maps_contains("libboost_thread.so.1.83.0") ? 1 : 0,
              address_ok ? 1 : 0);
  std::printf("provider_path=%s sizeof_once_flag=%zu trivial_destructor=%d\n",
              address_ok && info.dli_fname ? info.dli_fname : "NOT_OBSERVED",
              sizeof(boost::once_flag),
              std::is_trivially_destructible<boost::once_flag>::value ? 1 : 0);
  bool first = false;
  bool second = false;
  bool third = true;
  {
    boost::once_flag flag = BOOST_ONCE_INIT;
    first = boost::thread_detail::enter_once_region(flag);
    boost::thread_detail::rollback_once_region(flag);
    second = boost::thread_detail::enter_once_region(flag);
    boost::thread_detail::commit_once_region(flag);
    third = boost::thread_detail::enter_once_region(flag);
    std::printf("states=%d,%d,%d expected=1,1,0\n",
                first ? 1 : 0, second ? 1 : 0, third ? 1 : 0);
    std::puts("consumer_scope_exit_begin=YES");
  }
  std::puts("consumer_scope_exit_completed=YES");
  return first && second && !third ? 0 : 42;
}
