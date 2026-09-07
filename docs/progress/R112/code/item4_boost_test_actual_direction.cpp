#include <cstdio>
#include <cstring>
#include <dlfcn.h>
#include <string>

namespace {
bool maps_contains(const char* token) {
  std::FILE* maps = std::fopen("/proc/self/maps", "r");
  if (!maps) return false;
  char line[4096];
  while (std::fgets(line, sizeof(line), maps)) {
    if (std::strstr(line, token)) {
      std::fclose(maps);
      return true;
    }
  }
  std::fclose(maps);
  return false;
}

// Boost 1.83 basic_cstring<const char> consists of two pointers in the
// provider source and headers inspected by the earlier R110 probe.
struct input_type {
  const char* m_begin;
  const char* m_end;
  input_type(const char* begin, const char* end) : m_begin(begin), m_end(end) {}
  input_type(const input_type& other) : m_begin(other.m_begin), m_end(other.m_end) {}
};
static_assert(sizeof(input_type) == 2 * sizeof(const char*));
static_assert(alignof(input_type) == alignof(const char*));
}

int main(int argc, char** argv) {
  if (argc != 2) return 2;
  void* handle = dlopen(argv[1], RTLD_NOW | RTLD_LOCAL);
  if (!handle) {
    std::printf("dlopen_ok=0 error=%s\n", dlerror());
    return 3;
  }
  // The libc++-built provider does not use libstdc++'s B5cxx11 return tag.
  constexpr const char* symbol_name =
      "_ZN5boost9unit_test9ut_detail24normalize_test_case_nameENS0_13basic_cstringIKcEE";
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
  std::printf("provider_path=%s\n",
              address_ok && info.dli_fname ? info.dli_fname : "NOT_OBSERVED");
  std::fflush(stdout);

  using return_function = std::string (*)(input_type);
  auto function = reinterpret_cast<return_function>(symbol);
  const char input[] = "& alpha/beta+gamma ";
  const char expected[] = "alpha_beta_gamma";
  bool value_ok = false;
  {
    std::string value = function(input_type(input, input + sizeof(input) - 1));
    std::puts("provider_constructed=YES");
    std::fflush(stdout);
    const std::size_t observed_size = value.size();
    if (observed_size == sizeof(expected) - 1)
      value_ok = std::memcmp(value.data(), expected, sizeof(expected) - 1) == 0;
    std::printf("observed_size=%zu expected_size=%zu value_ok=%d\n",
                observed_size, sizeof(expected) - 1, value_ok ? 1 : 0);
    std::puts("consumer_destroy_begin=YES");
    std::fflush(stdout);
  }
  std::puts("consumer_destroy_completed=YES");
  std::fflush(stdout);
  dlclose(handle);
  std::puts("dlclose_completed=YES");
  return value_ok ? 0 : 41;
}
