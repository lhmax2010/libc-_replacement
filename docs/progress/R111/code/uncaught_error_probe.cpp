#include <condition_variable>
#include <cstdio>
#include <cstring>
#include <exception>
#include <execinfo.h>
#include <mutex>
#include <unistd.h>

namespace {

int cleanup_count = 0;

struct CleanupProbe {
  ~CleanupProbe() {
    ++cleanup_count;
    constexpr char marker[] = "event=cleanup\n";
    (void)!write(STDOUT_FILENO, marker, sizeof(marker) - 1);
  }
};

void terminate_with_trace() {
  char buffer[96];
  const int length = std::snprintf(buffer, sizeof(buffer), "event=terminate cleanup_count=%d\n", cleanup_count);
  if (length > 0)
    (void)!write(STDOUT_FILENO, buffer, static_cast<size_t>(length));
  void* frames[32];
  const int count = backtrace(frames, 32);
  backtrace_symbols_fd(frames, count, STDERR_FILENO);
  _Exit(86);
}

__attribute__((noinline)) void ordinary_unowned_error() {
  std::condition_variable condition;
  std::mutex mutex;
  std::unique_lock<std::mutex> lock(mutex, std::defer_lock);
  CleanupProbe cleanup;
  std::puts("event=before_wait owns_lock=0");
  condition.wait(lock);
  std::puts("event=unexpected_return");
}

} // namespace

int main(int argc, char** argv) {
  std::setvbuf(stdout, nullptr, _IONBF, 0);
  if (argc != 2)
    return 64;
  if (std::strcmp(argv[1], "instrumented") == 0)
    std::set_terminate(terminate_with_trace);
  else if (std::strcmp(argv[1], "default") != 0)
    return 64;
  ordinary_unowned_error();
  return 67;
}
