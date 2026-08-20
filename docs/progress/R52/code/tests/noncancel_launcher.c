#include <dlfcn.h>
#include <stdio.h>

typedef int (*entry_type)(const char*);

int main(int argc, char** argv) {
  if (argc != 3) return 64;
  void* handle = dlopen(argv[1], RTLD_NOW | RTLD_LOCAL);
  if (!handle) { fprintf(stderr, "DLOPEN_ERROR=%s\n", dlerror()); return 65; }
  entry_type entry = (entry_type)dlsym(handle, "r52_noncancel_run");
  if (!entry) { fprintf(stderr, "DLSYM_ERROR=%s\n", dlerror()); return 66; }
  int result = entry(argv[2]);
  dlclose(handle);
  return result;
}
