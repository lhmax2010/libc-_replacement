#include <cmath>
#include <cstdint>
#include <functional>

template <class T>
void accept(std::function<T(T)>) {}

int main() {
  accept<float>(static_cast<float (*)(float)>(std::abs));
  accept<std::int32_t>(
      static_cast<std::int32_t (*)(std::int32_t)>(std::abs));
}
