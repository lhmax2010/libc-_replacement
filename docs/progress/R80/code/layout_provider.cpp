#include "layout_boundary.h"

std::deque<int>* make_payload() {
  return new std::deque<int>{11, 22, 33, 44};
}

extern "C" std::size_t provider_size(const void* payload) {
  if (payload == nullptr)
    return sizeof(std::deque<int>);
  return static_cast<const std::deque<int>*>(payload)->size();
}

extern "C" void destroy_payload(void* payload) {
  delete static_cast<std::deque<int>*>(payload);
}
