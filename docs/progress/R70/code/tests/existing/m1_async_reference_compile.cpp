#include <future>

int value = 42;

int& return_reference() { return value; }

int main() {
  std::future<int&> result = std::async(std::launch::async, return_reference);
  return &result.get() == &value ? 0 : 1;
}
