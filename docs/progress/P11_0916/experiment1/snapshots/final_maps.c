#include <stdio.h>
#include <stdlib.h>
static void dump_maps(void) {
  FILE *f = fopen("/proc/self/maps", "r");
  char line[4096];
  fprintf(stderr, "P11_PROC_SELF_MAPS_BEGIN\n");
  if (!f) { perror("maps"); return; }
  while (fgets(line, sizeof line, f)) fputs(line, stderr);
  fclose(f);
  fprintf(stderr, "P11_PROC_SELF_MAPS_END\n");
}
__attribute__((constructor)) static void install(void) { atexit(dump_maps); }
