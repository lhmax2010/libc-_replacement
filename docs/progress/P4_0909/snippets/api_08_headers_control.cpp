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
std::unique_ptr<Widget> engine_create_widget();     // 谁来释放？
std::shared_ptr<Config> engine_get_config();        // 控制块在哪一侧？
