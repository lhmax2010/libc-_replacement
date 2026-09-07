#include <stdio.h>
int present(void);
int missing(void);
int main(int argc, char** argv) {
  (void)argv;
  printf("main_entered=YES present=%d\n", present());
  fflush(stdout);
  if (argc > 1) {
    printf("before_missing=YES\n");
    fflush(stdout);
    printf("missing=%d\n", missing());
  }
  return 0;
}
