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
// 这些跨边界是安全的
class Engine {
public:
    int    width() const;                    // 返回 int
    void   setPosition(int x, int y);        // 参数是 int
    bool   isReady() const;                  // 返回 bool
    double scale() const;                    // 返回 double
};

struct Rect { int x, y, w, h; };             // POD，布局由 C ABI 定
Rect   getBounds();                          // 按值返回 POD，安全
