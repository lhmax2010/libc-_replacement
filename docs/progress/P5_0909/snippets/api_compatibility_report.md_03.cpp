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
// 不安全 —— 参数和返回值都是 int，但异常会穿过去
int engine_compute(engine_handle h) {
    auto result = internal_cpp_function();   // 这里可能抛
    return result;
}

// 仅在上述前提下示意普通错误转换；与上一方案互斥
int engine_compute(engine_handle h, int* out) {
    if (out == nullptr) return -3;
    try {
        *out = internal_cpp_function();
        return 0;
    } catch (const std::exception&) {
        return -1;
    } catch (...) {
        return -2;
    }
}
