#include <shared_mutex>
static_assert(_GLIBCXX_USE_PTHREAD_RWLOCK_T == 1);
static_assert(_GTHREAD_USE_MUTEX_TIMEDLOCK == 1);
static_assert(sizeof(std::shared_mutex) == sizeof(pthread_rwlock_t));
int main() { std::shared_mutex lock; auto native=lock.native_handle(); return native ? 0 : 1; }
