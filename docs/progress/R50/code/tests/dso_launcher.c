extern void* dlopen(const char*, int);
extern void* dlsym(void*, const char*);
extern const char* dlerror(void);
extern int fprintf(void*, const char*, ...);
extern void* stderr;

#define RTLD_NOW 2
#define RTLD_GLOBAL 0x100
typedef int (*run_fn)(const char*);

int main(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "USAGE=%s consumer.so mode\n", argv[0]);
    return 64;
  }
  void* handle = dlopen(argv[1], RTLD_NOW | RTLD_GLOBAL);
  if (handle == 0) {
    fprintf(stderr, "DLOPEN_ERROR=%s\n", dlerror());
    return 65;
  }
  dlerror();
  run_fn run = (run_fn)dlsym(handle, "r50_run");
  const char* error = dlerror();
  if (error != 0) {
    fprintf(stderr, "DLSYM_ERROR=%s\n", error);
    return 66;
  }
  int rc = run(argv[2]);
  fprintf(stderr, "DSO_RUN_RETURN=%d\n", rc);
  return rc;
}
