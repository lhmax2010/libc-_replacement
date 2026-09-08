#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

__attribute__((constructor(101))) static void verify_runtime_identity() {
  setvbuf(stdout, nullptr, _IONBF, 0);
  alarm(30);
  const char* expected = getenv("EXPECTED_RUNTIME");
  if (!expected) _Exit(90);
  FILE* maps = fopen("/proc/self/maps", "r");
  if (!maps) _Exit(91);
  bool cxx = false, abi = false, wrong = false;
  char line[4096];
  while (fgets(line, sizeof(line), maps)) {
    if (strstr(line, "libc++.so") || strstr(line, "libc++abi.so")) {
      fputs(line, stdout);
      if (!strstr(line, expected)) wrong = true;
      if (strstr(line, "libc++.so")) cxx = true;
      if (strstr(line, "libc++abi.so")) abi = true;
    }
  }
  fclose(maps);
  printf("RUNTIME_MAPS cxx=%d abi=%d wrong=%d\n", cxx, abi, wrong);
  if (!cxx || !abi || wrong) _Exit(92);
  puts("RUNTIME_IDENTITY=PASS");
}
