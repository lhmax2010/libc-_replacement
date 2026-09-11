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
// 原实验共享类型；抛出与接收在不同翻译单元
struct BoundaryError { int code; };
// 提供方操作：throw BoundaryError{77};
// 消费方依次有 catch (const BoundaryError&) 和 catch (...)
