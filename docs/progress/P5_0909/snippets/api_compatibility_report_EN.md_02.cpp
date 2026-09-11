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
// Conditional interface illustration; these member calls have no corresponding cross-library measurement
class Engine {
public:
    int    width() const;                    // returns int
    void   setPosition(int x, int y);        // takes ints
    bool   isReady() const;                  // returns bool
    double scale() const;                    // returns double
};

struct Rect { int x, y, w, h; };             // check complete definitions and target ABI on both sides
Rect   getBounds();                          // check the by-value return ABI as well
