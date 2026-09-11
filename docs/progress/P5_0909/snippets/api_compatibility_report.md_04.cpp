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
// 不安全 —— 分配释放 API 不配对，同一套库也不能如此使用
char* engine_get_name(engine_handle h);   // 内部 new[] 出来的
// 应用侧：free(name);  ← 不调用 operator delete，不能释放 new[] 的结果

// 显式配对方案，仍须满足异常、取消及指针有效性等契约
char* engine_get_name(engine_handle h);
void  engine_free_string(char* s);        // 由提供方释放

// 或者由调用方提供缓冲区
int   engine_get_name(engine_handle h, char* buf, size_t buflen);
