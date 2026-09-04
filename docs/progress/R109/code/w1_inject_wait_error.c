#include <errno.h>
#include <pthread.h>
#include <unistd.h>

int pthread_cond_wait(pthread_cond_t* condition, pthread_mutex_t* mutex) {
  (void)condition;
  (void)mutex;
  static const char marker[] = "injector=called\n";
  (void)!write(STDERR_FILENO, marker, sizeof(marker) - 1);
  return EINVAL;
}
