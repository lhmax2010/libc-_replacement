#include <cmath>
#include <cstdint>
#include <functional>

template <class T>
void accept(std::function<T(T)>) {}

int main() {
  accept<float>(std::abs<float>);
  accept<std::int32_t>(std::abs<std::int32_t>);
}
