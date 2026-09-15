#pragma once
#include <cerrno>
#include <dlfcn.h>
#include <link.h>
#include <pthread.h>
// Explicit absolute path and root-DSO symbol ownership; never RTLD_DEFAULT/NEXT.
namespace p12_loader {
struct State {
  pthread_once_t once = PTHREAD_ONCE_INIT;
  void* handle = nullptr;
  void* symbol = nullptr;
  int error = ENOENT;
};
inline int resolve(State& state, const char* path, const char* symbol) {
  if (!path || path[0] != '/' || !symbol) return EINVAL;
  state.handle = dlopen(path, RTLD_NOW | RTLD_LOCAL | RTLD_NODELETE);
  if (!state.handle) return ENOENT;
  dlerror(); void* address = dlsym(state.handle, symbol);
  if (dlerror() || !address) return ENOSYS;
  link_map* object = nullptr; Dl_info info{};
  if (dlinfo(state.handle, RTLD_DI_LINKMAP, &object) || !object || !dladdr(address, &info)) return ELIBBAD;
  if (reinterpret_cast<void*>(object->l_addr) != info.dli_fbase) return ELIBBAD;
  Dl_info owner{};
  if (dladdr(&state, &owner) && owner.dli_fbase == info.dli_fbase) return ELOOP;
  state.symbol = address;
  return 0;
}
// Handle stays loaded until process exit; no unload while callbacks are live.
}
