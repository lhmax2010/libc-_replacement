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
// Original shared type; throwing and receiving code are in separate translation units.
struct BoundaryError { int code; };
// Provider operation: throw BoundaryError{77};
// Consumer has catch (const BoundaryError&) followed by catch (...).
