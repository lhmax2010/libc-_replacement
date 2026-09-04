#include <boost/test/tree/test_unit.hpp>

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <dlfcn.h>
#include <fstream>
#include <string>

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

int main(int argc, char** argv) {
  if (argc != 2) return 2;
  void* handle = dlopen(argv[1], RTLD_NOW | RTLD_LOCAL);
  if (!handle) {
    std::printf("dlopen_ok=0 error=%s\n", dlerror());
    return 3;
  }
  constexpr const char* symbol_name =
      "_ZN5boost9unit_test9ut_detail24normalize_test_case_nameB5cxx11ENS0_13basic_cstringIKcEE";
  void* symbol = dlsym(handle, symbol_name);
  if (!symbol) {
    std::printf("dlsym_ok=0 error=%s\n", dlerror());
    return 4;
  }
  Dl_info info{};
  const int address_ok = dladdr(symbol, &info);
  std::printf("runtime_maps=libcxx:%d,libstdcxx:%d,provider:%d dladdr=%d\n",
              maps_contains("libc++.so.1") ? 1 : 0,
              maps_contains("libstdc++.so.6") ? 1 : 0,
              maps_contains("libboost_unit_test_framework.so.1.83.0") ? 1 : 0,
              address_ok ? 1 : 0);
  std::printf("provider_path=%s\n", address_ok && info.dli_fname ? info.dli_fname : "NOT_OBSERVED");

  using input_type = boost::unit_test::const_string;
  using raw_return_function = void (*)(void*, input_type);
  auto function = reinterpret_cast<raw_return_function>(symbol);
  const char input[] = "& alpha/beta+gamma ";
  const char expected[] = "alpha_beta_gamma";
  alignas(std::max_align_t) unsigned char storage[64]{};
  function(storage, input_type(input, sizeof(input) - 1));
  std::puts("provider_constructed=YES");
  std::fflush(stdout);

  auto* value = reinterpret_cast<std::string*>(storage);
  const std::size_t observed_size = value->size();
  bool value_ok = false;
  if (observed_size == sizeof(expected) - 1) {
    value_ok = std::memcmp(value->data(), expected, sizeof(expected) - 1) == 0;
  }
  std::printf("observed_size=%zu expected_size=%zu value_ok=%d\n",
              observed_size, sizeof(expected) - 1, value_ok ? 1 : 0);
  if (observed_size == sizeof(expected) - 1) {
    std::fputs("observed_hex=", stdout);
    for (std::size_t index = 0; index < observed_size; ++index)
      std::printf("%02x", static_cast<unsigned char>(value->data()[index]));
    std::fputc('\n', stdout);
  } else {
    std::puts("observed_hex=NOT_OBSERVED_UNEXPECTED_SIZE");
  }
  std::puts("consumer_destroy_begin=YES");
  std::fflush(stdout);
  value->~basic_string();
  std::puts("consumer_destroy_completed=YES");
  std::fflush(stdout);
  dlclose(handle);
  std::puts("dlclose_completed=YES");
  return value_ok ? 0 : 41;
}
