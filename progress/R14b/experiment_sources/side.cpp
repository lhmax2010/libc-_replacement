#include "shared.hpp"

#if defined(R14B_NEW_SIDE)
#define R14B_SIDE_ID 1
#define R14B_SET_OBSERVER new_set_observer
#define R14B_THROW new_throw
#define R14B_CATCH new_catch
#define R14B_THROW_EVENT "THROW:new"
#define R14B_INNER_EVENT "DTOR:new:inner"
#define R14B_RETHROW_EVENT "RETHROW:new"
#define R14B_OUTER_EVENT "DTOR:new:outer"
#define R14B_CATCH_NEW_EVENT "CATCH:new:new"
#define R14B_CATCH_LEGACY_EVENT "CATCH:new:legacy"
#define R14B_CATCH_OTHER_EVENT "CATCH:new:other"
#elif defined(R14B_LEGACY_SIDE)
#define R14B_SIDE_ID 2
#define R14B_SET_OBSERVER legacy_set_observer
#define R14B_THROW legacy_throw
#define R14B_CATCH legacy_catch
#define R14B_THROW_EVENT "THROW:legacy"
#define R14B_INNER_EVENT "DTOR:legacy:inner"
#define R14B_RETHROW_EVENT "RETHROW:legacy"
#define R14B_OUTER_EVENT "DTOR:legacy:outer"
#define R14B_CATCH_NEW_EVENT "CATCH:legacy:new"
#define R14B_CATCH_LEGACY_EVENT "CATCH:legacy:legacy"
#define R14B_CATCH_OTHER_EVENT "CATCH:legacy:other"
#else
#error "Select exactly one R14b side"
#endif

namespace {
r14b_observer_fn observer;

__attribute__((noinline)) void emit(const char *event) {
  if (observer)
    observer(event);
}

struct Cleanup {
  const char *event;
  ~Cleanup() { emit(event); }
};
} // namespace

extern "C" __attribute__((visibility("default"))) void
R14B_SET_OBSERVER(r14b_observer_fn fn) {
  observer = fn;
}

extern "C" __attribute__((visibility("default"), noinline)) void R14B_THROW() {
  Cleanup outer{R14B_OUTER_EVENT};
  try {
    Cleanup inner{R14B_INNER_EVENT};
    emit(R14B_THROW_EVENT);
    throw R14B_SIDE_ID;
  } catch (...) {
    emit(R14B_RETHROW_EVENT);
    throw;
  }
}

extern "C" __attribute__((visibility("default"), noinline)) int
R14B_CATCH(r14b_throw_fn thrower) {
  try {
    thrower();
  } catch (int origin) {
    if (origin == 1)
      emit(R14B_CATCH_NEW_EVENT);
    else if (origin == 2)
      emit(R14B_CATCH_LEGACY_EVENT);
    else
      emit(R14B_CATCH_OTHER_EVENT);
    return origin;
  } catch (...) {
    emit(R14B_CATCH_OTHER_EVENT);
    return -99;
  }
  return -100;
}
