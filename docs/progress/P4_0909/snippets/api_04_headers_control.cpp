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
// 不安全 —— 一侧分配，另一侧释放
char* engine_get_name(engine_handle h);   // 内部 new[] 出来的
// 应用侧：free(name);  ← 用的是另一套 operator delete

// 安全 —— 提供配对的释放函数
char* engine_get_name(engine_handle h);
void  engine_free_string(char* s);        // 由提供方释放

// 或者由调用方提供缓冲区
int   engine_get_name(engine_handle h, char* buf, size_t buflen);
