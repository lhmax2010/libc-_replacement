#include "bridge.h"
#include <vector>
P12_EXPORT int p12_send_reply(int32_t instance, const std::vector<uint8_t>& bytes) {
  return p12_vector_bytes(instance, bytes.data(), bytes.size());
}
