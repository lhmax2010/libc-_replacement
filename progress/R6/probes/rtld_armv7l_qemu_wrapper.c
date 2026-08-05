#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  static const char qemu[] = "/usr/bin/qemu-arm-static";
  static const char scratch[] = "/home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260805-r3b/local/BUILD-ROOTS/scratch.armv7l.0";
  static const char rtld[] = "/home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260805-r3b/local/BUILD-ROOTS/scratch.armv7l.0/lib/ld-linux.so.3";
  static const char ld_path[] = "/home/toolchain/development/libc++_replacement/tmp/R6/elf-extract-armv7l-r3/usr/lib:/home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260805-r3b/local/BUILD-ROOTS/scratch.armv7l.0/usr/lib:/home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260805-r3b/local/BUILD-ROOTS/scratch.armv7l.0/lib";
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
