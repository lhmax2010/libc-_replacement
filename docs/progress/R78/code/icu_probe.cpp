#include <cstdio>
#include <cstring>
#include <dlfcn.h>
#include <link.h>
#include <string>

namespace {
int library_callback(dl_phdr_info* info, size_t, void*) {
  if (info->dlpi_name &&
      (std::strstr(info->dlpi_name, "libicuuc") ||
       std::strstr(info->dlpi_name, "libc++.so") ||
       std::strstr(info->dlpi_name, "libc++abi.so") ||
       std::strstr(info->dlpi_name, "libstdc++.so")))
    std::printf("LOADED=%s\n", info->dlpi_name);
  return 0;
}
}  // namespace

int main() {
  void* handle = dlopen("/usr/lib/libicuuc.so.78.1", RTLD_NOW | RTLD_GLOBAL);
  if (!handle) {
    std::fprintf(stderr, "DLOPEN_ERROR=%s\n", dlerror());
    return 3;
  }
  auto get_version = reinterpret_cast<void (*)(unsigned char*)>(
      dlsym(handle, "u_getVersion"));
  if (!get_version) {
    std::fprintf(stderr, "DLSYM_ERROR=%s\n", dlerror());
    return 4;
  }
  unsigned char version[4] = {};
  get_version(version);
  dl_iterate_phdr(library_callback, nullptr);
  bool ok = version[0] == 78;
  std::string formatted = std::to_string(version[0]) + "." +
                          std::to_string(version[1]) + "." +
                          std::to_string(version[2]) + "." +
                          std::to_string(version[3]);
  std::printf("SCENARIO=installed-icu-c-api VERSION=%s ASSERT=%s\n",
              formatted.c_str(), ok ? "PASS" : "FAIL");
  return ok ? 0 : 5;
}
