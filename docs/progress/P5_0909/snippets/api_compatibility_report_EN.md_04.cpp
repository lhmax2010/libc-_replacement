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
// Not safe — mismatched allocation/release APIs, even within one library
char* engine_get_name(engine_handle h);   // internally new[]
// application side: free(name);          // does not call operator delete; invalid for a new[] result

// Explicit pairing; exception, cancellation and pointer-validity contracts still apply
char* engine_get_name(engine_handle h);
void  engine_free_string(char* s);        // freed by the provider

// Or let the caller supply the buffer
int   engine_get_name(engine_handle h, char* buf, size_t buflen);
