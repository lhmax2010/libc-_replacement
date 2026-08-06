#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  static const char qemu[] = "/usr/bin/qemu-arm64-static";
  static const char scratch[] = "/home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/LIBCXX-2218-aarch64-20260806-c2/local/BUILD-ROOTS/scratch.aarch64.0";
  static const char rtld[] = "/home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/LIBCXX-2218-aarch64-20260806-c2/local/BUILD-ROOTS/scratch.aarch64.0/usr/lib64/ld-linux-aarch64.so.1";
  static const char ld_path[] = "/home/toolchain/development/libc++_replacement/progress/R9C/seg2/gates/aarch64/static/extracted/usr/lib64:/home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/LIBCXX-2218-aarch64-20260806-c2/local/BUILD-ROOTS/scratch.aarch64.0/usr/lib64:/home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/LIBCXX-2218-aarch64-20260806-c2/local/BUILD-ROOTS/scratch.aarch64.0/lib64";
  char **child_argv = calloc((size_t)argc + 4, sizeof(*child_argv));
  if (child_argv == NULL) {
    perror("calloc");
    return 125;
  }
  if (setenv("LD_LIBRARY_PATH", ld_path, 1) != 0) {
    perror("setenv");
    return 125;
  }
  child_argv[0] = (char *)qemu;
  child_argv[1] = "-L";
  child_argv[2] = (char *)scratch;
  child_argv[3] = (char *)rtld;
  for (int i = 1; i < argc; ++i)
    child_argv[i + 3] = argv[i];
  child_argv[argc + 3] = NULL;
  execv(qemu, child_argv);
  fprintf(stderr, "execv(%s) failed: errno=%d\n", qemu, errno);
  return 126;
}
