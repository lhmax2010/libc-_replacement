#include <cstdio>
#include <cstring>
#include <dlfcn.h>
#include <link.h>
#include <string>

namespace {
int library_callback(dl_phdr_info* info, size_t, void*) {
  if (info->dlpi_name &&
      (std::strstr(info->dlpi_name, "libpen-wave") ||
       std::strstr(info->dlpi_name, "libc++.so") ||
       std::strstr(info->dlpi_name, "libc++abi.so") ||
       std::strstr(info->dlpi_name, "libstdc++.so")))
    std::printf("LOADED=%s\n", info->dlpi_name);
  return 0;
}
}  // namespace

int main(int argc, char** argv) {
  if (argc != 2) {
    std::fprintf(stderr, "usage: %s LIBPEN_WAVE\n", argv[0]);
    return 2;
  }
  void* handle = dlopen(argv[1], RTLD_NOW | RTLD_GLOBAL);
  if (!handle) {
    std::fprintf(stderr, "DLOPEN_ERROR=%s\n", dlerror());
    return 3;
  }
  auto set_path = reinterpret_cast<void (*)(const char*)>(dlsym(handle, "SetResourcePath"));
  auto get_path = reinterpret_cast<const char* (*)()>(dlsym(handle, "GetResourcePath"));
  if (!set_path || !get_path) {
    std::fprintf(stderr, "DLSYM_ERROR=%s\n", dlerror());
    return 4;
  }
  const std::string expected = "/tmp/r78-pen-wave-resource";
  set_path(expected.c_str());
  const char* observed = get_path();
  dl_iterate_phdr(library_callback, nullptr);
  bool ok = observed && expected == observed;
  std::printf("SCENARIO=pen-wave-c-api OBSERVED=%s ASSERT=%s\n",
              observed ? observed : "(null)", ok ? "PASS" : "FAIL");
  return ok ? 0 : 5;
}
