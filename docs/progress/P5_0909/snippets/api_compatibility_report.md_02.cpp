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
// 条件性接口示意，以下成员调用未作对应跨库实测
class Engine {
public:
    int    width() const;                    // 返回 int
    void   setPosition(int x, int y);        // 参数是 int
    bool   isReady() const;                  // 返回 bool
    double scale() const;                    // 返回 double
};

struct Rect { int x, y, w, h; };             // 两侧须核对完整定义与目标 ABI
Rect   getBounds();                          // 按值返回的 ABI 也须核对
