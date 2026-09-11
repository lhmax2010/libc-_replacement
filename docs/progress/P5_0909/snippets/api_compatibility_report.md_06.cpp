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
// 提供方（libstdc++ 构建）
std::deque<int>* make_payload() {
    return new std::deque<int>{11, 22, 33, 44};
}

// 消费方（libc++ 构建）
int main() {
    auto* payload = make_payload();
    const auto producer_view = provider_size(payload);
    const auto consumer_view = payload->size();
    // 原程序在此打印两侧 sizeof 和长度视图
    destroy_payload(payload);       // 由提供方 delete
    return producer_view == consumer_view ? 0 : 42;
}
