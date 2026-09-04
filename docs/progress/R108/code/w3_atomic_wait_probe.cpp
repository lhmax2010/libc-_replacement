#include <atomic>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <pthread.h>
#include <sched.h>
#include <sys/wait.h>
#include <unistd.h>

namespace {
std::atomic<int> gate{0};
std::atomic<int> ready{0};
int marker_fd = -1;

constexpr char kTerminateMarker[] = "TERMINATE\n";
constexpr char kUnexpectedMarker[] = "UNEXPECTED_RETURN\n";

void write_all(int fd, const char* data, size_t size) {
  while (size != 0) {
    const ssize_t written = ::write(fd, data, size);
    if (written > 0) {
      data += written;
      size -= static_cast<size_t>(written);
      continue;
    }
    if (written < 0 && errno == EINTR) continue;
    break;
  }
}

void terminate_marker() {
  write_all(marker_fd, kTerminateMarker, sizeof(kTerminateMarker) - 1);
  ::_Exit(86);
}

void* waiting_worker(void*) {
  int old_type = 0;
  if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old_type) != 0)
    return reinterpret_cast<void*>(1);
  ready.store(1, std::memory_order_release);
  gate.wait(0, std::memory_order_acquire);
  return nullptr;
}

bool normal_control() {
  gate.store(0, std::memory_order_relaxed);
  ready.store(0, std::memory_order_relaxed);
  pthread_t thread{};
  if (pthread_create(&thread, nullptr, waiting_worker, nullptr) != 0) return false;
  while (ready.load(std::memory_order_acquire) == 0) sched_yield();
  gate.store(1, std::memory_order_release);
  gate.notify_one();
  void* result = reinterpret_cast<void*>(1);
  return pthread_join(thread, &result) == 0 && result == nullptr;
}

bool cancellation_case(int iteration) {
  int pipefd[2]{};
  if (pipe(pipefd) != 0) return false;
  const pid_t child = fork();
  if (child < 0) return false;
  if (child == 0) {
    close(pipefd[0]);
    marker_fd = pipefd[1];
    std::set_terminate(terminate_marker);
    gate.store(0, std::memory_order_relaxed);
    ready.store(0, std::memory_order_relaxed);
    pthread_t thread{};
    if (pthread_create(&thread, nullptr, waiting_worker, nullptr) != 0) _Exit(72);
    while (ready.load(std::memory_order_acquire) == 0) sched_yield();
    usleep(100000);
    if (pthread_cancel(thread) != 0) _Exit(73);
    void* result = nullptr;
    if (pthread_join(thread, &result) != 0) _Exit(74);
    write_all(marker_fd, kUnexpectedMarker, sizeof(kUnexpectedMarker) - 1);
    _Exit(71);
  }

  close(pipefd[1]);
  char observed[64]{};
  size_t used = 0;
  while (used < sizeof(observed)) {
    const ssize_t n = read(pipefd[0], observed + used, sizeof(observed) - used);
    if (n > 0) used += static_cast<size_t>(n);
    else if (n < 0 && errno == EINTR) continue;
    else break;
  }
  close(pipefd[0]);
  int status = 0;
  if (waitpid(child, &status, 0) != child) return false;
  const bool marker_ok = used == sizeof(kTerminateMarker) - 1 &&
      memcmp(observed, kTerminateMarker, sizeof(kTerminateMarker) - 1) == 0;
  const bool exit_ok = WIFEXITED(status) && WEXITSTATUS(status) == 86;
  std::printf("case=%d marker=%s exit=%s\n", iteration,
              marker_ok ? "yes" : "no", exit_ok ? "86" : "other");
  return marker_ok && exit_ok;
}
}  // namespace

int main() {
  if (!normal_control()) {
    std::puts("normal_control=FAIL");
    return 2;
  }
  std::puts("normal_control=PASS");
  int passed = 0;
  for (int i = 1; i <= 5; ++i) passed += cancellation_case(i) ? 1 : 0;
  std::printf("cancellation_cases=%d/5\n", passed);
  return passed == 5 ? 0 : 1;
}
