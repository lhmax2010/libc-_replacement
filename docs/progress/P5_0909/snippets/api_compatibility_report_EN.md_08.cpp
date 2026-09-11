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
// Cannot cross the boundary
std::unique_ptr<Widget> engine_create_widget();     // who frees it?
std::shared_ptr<Config> engine_get_config();        // where is the control block?
