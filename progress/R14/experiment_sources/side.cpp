#include "shared.hpp"

#if defined(R14_NEW_SIDE)
#define R14_SIDE_ID 1
#define R14_SET_OBSERVER new_set_observer
#define R14_THROW new_throw
#define R14_CATCH new_catch
#define R14_THROW_EVENT "THROW:new"
#define R14_INNER_EVENT "DTOR:new:inner"
#define R14_RETHROW_EVENT "RETHROW:new"
#define R14_OUTER_EVENT "DTOR:new:outer"
#define R14_CATCH_NEW_EVENT "CATCH:new:new"
#define R14_CATCH_LEGACY_EVENT "CATCH:new:legacy"
#define R14_CATCH_OTHER_EVENT "CATCH:new:other"
#elif defined(R14_LEGACY_SIDE)
#define R14_SIDE_ID 2
#define R14_SET_OBSERVER legacy_set_observer
#define R14_THROW legacy_throw
#define R14_CATCH legacy_catch
#define R14_THROW_EVENT "THROW:legacy"
#define R14_INNER_EVENT "DTOR:legacy:inner"
#define R14_RETHROW_EVENT "RETHROW:legacy"
#define R14_OUTER_EVENT "DTOR:legacy:outer"
#define R14_CATCH_NEW_EVENT "CATCH:legacy:new"
#define R14_CATCH_LEGACY_EVENT "CATCH:legacy:legacy"
#define R14_CATCH_OTHER_EVENT "CATCH:legacy:other"
#else
#error "Select exactly one R14 side"
#endif

namespace {
r14_observer_fn observer;

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
R14_SET_OBSERVER(r14_observer_fn fn) {
  observer = fn;
}

extern "C" __attribute__((visibility("default"), noinline)) void R14_THROW() {
  Cleanup outer{R14_OUTER_EVENT};
  try {
    Cleanup inner{R14_INNER_EVENT};
    emit(R14_THROW_EVENT);
    throw CrossError{R14_SIDE_ID};
  } catch (...) {
    emit(R14_RETHROW_EVENT);
    throw;
  }
}

extern "C" __attribute__((visibility("default"), noinline)) int
R14_CATCH(r14_throw_fn thrower) {
  try {
    thrower();
  } catch (const CrossError &error) {
    if (error.origin == 1)
      emit(R14_CATCH_NEW_EVENT);
    else if (error.origin == 2)
      emit(R14_CATCH_LEGACY_EVENT);
    else
      emit(R14_CATCH_OTHER_EVENT);
    return error.origin;
  } catch (...) {
    emit(R14_CATCH_OTHER_EVENT);
    return -99;
  }
  return -100;
}
