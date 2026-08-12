#include <rlottie.h>
#include <memory>

int main() {
  std::unique_ptr<rlottie::Animation> animation =
      rlottie::Animation::loadFromData("{}", "r29");
  return animation ? 0 : 0;
}
