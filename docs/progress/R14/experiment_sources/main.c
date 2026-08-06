#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*observer_fn)(const char *event);
typedef void (*throw_fn)(void);
typedef int (*catch_fn)(throw_fn thrower);
typedef void (*set_observer_fn)(observer_fn observer);

static const char *events[32];
static size_t event_count;

static void observe(const char *event) {
  if (event_count < sizeof(events) / sizeof(events[0]))
    events[event_count++] = event;
  printf("EVENT\t%zu\t%s\n", event_count, event);
}

static void fail_dl(const char *operation) {
  const char *error = dlerror();
  fprintf(stderr, "DYNAMIC_LOADER_ERROR\t%s\t%s\n", operation,
          error ? error : "NO_ERROR_TEXT");
  exit(70);
}

static void *load_one(const char *name, int flags, const char *mode) {
  void *handle;
  dlerror();
  if (strcmp(mode, "dt_needed") == 0)
    handle = dlopen(name, flags | RTLD_NOLOAD);
  else
    handle = dlopen(name, flags);
  if (!handle)
    fail_dl(name);
  return handle;
}

static void *required_symbol(void *handle, const char *name) {
  dlerror();
  void *symbol = dlsym(handle, name);
  if (!symbol)
    fail_dl(name);
  return symbol;
}

static int expected_event(size_t index, const char *value) {
  return index < event_count && strcmp(events[index], value) == 0;
}

int main(int argc, char **argv) {
  if (argc != 6) {
    fprintf(stderr,
            "usage: %s SCHEME MODE ORDER SCOPE DIRECTION\n",
            argv[0]);
    return 64;
  }

  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  const char *scheme = argv[1];
  const char *mode = argv[2];
  const char *order = argv[3];
  const char *scope = argv[4];
  const char *direction = argv[5];
  int flags = RTLD_LAZY;
  if (strcmp(scope, "global") == 0)
    flags |= RTLD_GLOBAL;
  else if (strcmp(scope, "local") == 0)
    flags |= RTLD_LOCAL;
  else if (strcmp(scope, "na") != 0) {
    fprintf(stderr, "INVALID_SCOPE\t%s\n", scope);
    return 64;
  }

  char new_name[64];
  snprintf(new_name, sizeof(new_name), "libnew_%s.so", scheme);
  const char *legacy_name = "liblegacy.so";
  void *new_handle;
  void *legacy_handle;

  printf("CASE\tscheme=%s\tmode=%s\torder=%s\tscope=%s\tdirection=%s\n",
         scheme, mode, order, scope, direction);

  if (strcmp(order, "legacy_new") == 0) {
    legacy_handle = load_one(legacy_name, flags, mode);
    new_handle = load_one(new_name, flags, mode);
  } else if (strcmp(order, "new_legacy") == 0) {
    new_handle = load_one(new_name, flags, mode);
    legacy_handle = load_one(legacy_name, flags, mode);
  } else {
    fprintf(stderr, "INVALID_ORDER\t%s\n", order);
    return 64;
  }

  set_observer_fn new_set_observer =
      (set_observer_fn)required_symbol(new_handle, "new_set_observer");
  set_observer_fn legacy_set_observer =
      (set_observer_fn)required_symbol(legacy_handle, "legacy_set_observer");
  throw_fn new_throw = (throw_fn)required_symbol(new_handle, "new_throw");
  throw_fn legacy_throw =
      (throw_fn)required_symbol(legacy_handle, "legacy_throw");
  catch_fn new_catch = (catch_fn)required_symbol(new_handle, "new_catch");
  catch_fn legacy_catch =
      (catch_fn)required_symbol(legacy_handle, "legacy_catch");
  new_set_observer(observe);
  legacy_set_observer(observe);

  throw_fn thrower;
  catch_fn catcher;
  const char *throw_side;
  const char *catch_side;
  int expected_origin;
  if (strcmp(direction, "new_new") == 0) {
    thrower = new_throw;
    catcher = new_catch;
    throw_side = "new";
    catch_side = "new";
    expected_origin = 1;
  } else if (strcmp(direction, "legacy_legacy") == 0) {
    thrower = legacy_throw;
    catcher = legacy_catch;
    throw_side = "legacy";
    catch_side = "legacy";
    expected_origin = 2;
  } else if (strcmp(direction, "new_legacy") == 0) {
    thrower = new_throw;
    catcher = legacy_catch;
    throw_side = "new";
    catch_side = "legacy";
    expected_origin = 1;
  } else if (strcmp(direction, "legacy_new") == 0) {
    thrower = legacy_throw;
    catcher = new_catch;
    throw_side = "legacy";
    catch_side = "new";
    expected_origin = 2;
  } else {
    fprintf(stderr, "INVALID_DIRECTION\t%s\n", direction);
    return 64;
  }

  int caught_origin = catcher(thrower);
  char expected[5][64];
  snprintf(expected[0], sizeof(expected[0]), "THROW:%s", throw_side);
  snprintf(expected[1], sizeof(expected[1]), "DTOR:%s:inner", throw_side);
  snprintf(expected[2], sizeof(expected[2]), "RETHROW:%s", throw_side);
  snprintf(expected[3], sizeof(expected[3]), "DTOR:%s:outer", throw_side);
  snprintf(expected[4], sizeof(expected[4]), "CATCH:%s:%s", catch_side,
           throw_side);

  int type_ok = caught_origin == expected_origin;
  int count_ok = event_count == 5;
  int order_ok = count_ok;
  for (size_t i = 0; i < 5 && order_ok; ++i)
    order_ok = expected_event(i, expected[i]);

  printf("RESULT\tcaught_origin=%d\texpected_origin=%d\n", caught_origin,
         expected_origin);
  printf("CHECK\ttype=%s\tcleanup_count=%s\torder=%s\tevent_count=%zu\n",
         type_ok ? "PASS" : "FAIL", count_ok ? "PASS" : "FAIL",
         order_ok ? "PASS" : "FAIL", event_count);
  return type_ok && count_ok && order_ok ? 0 : 1;
}
