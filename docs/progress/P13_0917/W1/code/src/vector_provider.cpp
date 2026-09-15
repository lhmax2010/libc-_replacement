// Constructed DATA-LAYER receiver, not a browser/plugin implementation.
#include "bridge.h"
#include <vector>
namespace { std::vector<uint8_t> saved; int32_t saved_instance; }
P12_EXPORT int p12_send_reply(int32_t instance, const std::vector<uint8_t>& bytes) {
  saved = bytes; saved_instance = instance; return 1;
}
P12_EXPORT int p12_check_reply(int32_t instance, const uint8_t* bytes, size_t size) {
  if (saved_instance != instance || saved.size() != size) return 0;
  for (size_t i = 0; i < size; ++i) if (saved[i] != bytes[i]) return 0;
  return 1;
}
P12_EXPORT size_t p12_reset_reply() {
  std::vector<uint8_t>().swap(saved);
  return saved.size() + saved.capacity();
}
