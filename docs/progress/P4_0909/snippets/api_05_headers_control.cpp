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
// 不能跨边界
std::string  engine_get_name();                    // 返回 string
void         engine_set_items(std::vector<int>);   // 参数是 vector
std::map<int, std::string> engine_get_config();    // 返回 map
