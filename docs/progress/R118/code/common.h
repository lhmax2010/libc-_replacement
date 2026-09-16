#pragma once
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#include <string>

inline void identity() {
#ifdef _LIBCPP_VERSION
  std::printf("LIB=libc++ VERSION=%d ABI=%d\n", _LIBCPP_VERSION, _LIBCPP_ABI_VERSION);
#else
  std::printf("LIB=libstdc++ VERSION=%d DATE=%d CXX11_ABI=%d\n", _GLIBCXX_RELEASE, __GLIBCXX__, _GLIBCXX_USE_CXX11_ABI);
#endif
  std::printf("CPLUSPLUS=%ld POINTER_BYTES=%zu\n", long(__cplusplus), sizeof(void*));
  FILE* f = std::fopen("/proc/self/maps", "r"); assert(f);
  char line[2048]; int count=0;
  while (std::fgets(line, sizeof line, f)) {
    if (std::strstr(line,"libc++") || std::strstr(line,"libstdc++")) {
      std::printf("MAP %s",line); ++count;
    }
  }
  std::fclose(f); assert(count>0);
}
inline void bytes(const char* label, const std::string& s) {
  std::printf("%s size=%zu hex=", label,s.size());
  for (unsigned char c:s) std::printf("%02x",unsigned(c));
  std::printf("\n");
}
