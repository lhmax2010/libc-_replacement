#include <cstddef>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <deque>
#include <memory>
#include <stdexcept>
typedef struct engine_s* engine_handle;
int internal_cpp_function();
struct Widget; struct Config;
extern "C" std::size_t provider_size(const void*);
extern "C" void destroy_payload(void*);
// Provider, built against libstdc++
std::deque<int>* make_payload() {
    return new std::deque<int>{11, 22, 33, 44};
}

// Consumer, built against libc++
int main() {
    auto* payload = make_payload();
    const auto producer_view = provider_size(payload);
    const auto consumer_view = payload->size();
    // The original program prints both sizeof values and length views here.
    destroy_payload(payload);       // deleted by the provider
    return producer_view == consumer_view ? 0 : 42;
}
