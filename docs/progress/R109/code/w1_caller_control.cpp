#include <condition_variable>
#include <mutex>

void control(std::condition_variable& condition, std::mutex& mutex) {
  std::unique_lock<std::mutex> held(mutex);
  condition.wait(held);
  std::unique_lock<std::mutex> unheld(mutex, std::defer_lock);
  condition.wait(unheld);
}
