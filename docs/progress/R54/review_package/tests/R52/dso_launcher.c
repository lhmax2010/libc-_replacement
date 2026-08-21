#include <dlfcn.h>
#include <stdio.h>

typedef int (*run_fn)(const char*);

int main(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "USAGE=%s DSO MODE\n", argv[0]);
    return 64;
  }
  void* handle = dlopen(argv[1], RTLD_NOW | RTLD_LOCAL);
  if (!handle) {
    fprintf(stderr, "DLOPEN_ERROR=%s\n", dlerror());
    return 65;
  }
  run_fn run = (run_fn)dlsym(handle, "r52_run");
  if (!run) {
    fprintf(stderr, "DLSYM_ERROR=%s\n", dlerror());
    return 66;
  }
  const int rc = run(argv[2]);
  dlclose(handle);
  return rc;
}
