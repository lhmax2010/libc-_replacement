#include "layout_boundary.h"

#include <cstdio>

int main() {
  std::deque<int>* payload = make_payload();
  const std::size_t producer_view = provider_size(payload);
  const std::size_t consumer_view = payload->size();
  std::printf("provider_sizeof=%zu\n", provider_size(nullptr));
  std::printf("consumer_sizeof=%zu\n", sizeof(std::deque<int>));
  std::printf("provider_view=%zu\n", producer_view);
  std::printf("consumer_view=%zu\n", consumer_view);
  destroy_payload(payload);
  return producer_view == consumer_view ? 0 : 42;
}
