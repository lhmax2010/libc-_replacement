#ifndef W3_PROVIDER_TOKEN
#error "W3_PROVIDER_TOKEN must name the provider DSO"
#endif

#include <cstdio>
#include <cstring>

namespace w3_maps_probe {
static bool contains(const char* token) {
  std::FILE* maps = std::fopen("/proc/self/maps", "r");
  if (!maps) return false;
  bool found = false;
  char line[4096];
  while (std::fgets(line, sizeof(line), maps)) {
    if (std::strstr(line, token)) {
      found = true;
      break;
    }
  }
  std::fclose(maps);
  return found;
}

__attribute__((constructor)) static void record_loaded_objects() {
  std::printf("runtime_maps=libcxx:%d,libstdcxx:%d,provider:%d token=%s\n",
              contains("libc++.so.1") ? 1 : 0,
              contains("libstdc++.so.6") ? 1 : 0,
              contains(W3_PROVIDER_TOKEN) ? 1 : 0,
              W3_PROVIDER_TOKEN);
  std::fflush(stdout);
}
}
