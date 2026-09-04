#include <condition_variable>
#include <cstring>
#include <mutex>

int main(int argc, char** argv) {
  if (argc != 2) return 2;
  std::condition_variable condition;
  std::mutex mutex;
  if (std::strcmp(argv[1], "unowned") == 0) {
    std::unique_lock<std::mutex> lock(mutex, std::defer_lock);
    condition.wait(lock);
    return 90;
  }
  if (std::strcmp(argv[1], "backend") == 0) {
    std::unique_lock<std::mutex> lock(mutex);
    condition.wait(lock);
    return 91;
  }
  return 2;
}
