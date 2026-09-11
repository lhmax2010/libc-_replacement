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
// Not safe — parameters and return are int, but exceptions still cross
int engine_compute(engine_handle h) {
    auto result = internal_cpp_function();   // may throw
    return result;
}

// Ordinary-error illustration under the stated conditions; an alternative to the preceding interface
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
