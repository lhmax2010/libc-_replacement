#include "bridge.h"
#include <ewk_cookie_parser.h>
#include <cerrno>
#include <cstdio>
#include <new>
#include <stdexcept>
#ifdef P12_TESTING
static thread_local int fail_field = -1;
P12_EXPORT void p12_test_fail_field(int i) { fail_field = i; }
#endif
static int emit(void* ptr, unsigned i, const char* bytes, size_t size) {
  if (!ptr || i >= 4 || (!bytes && size)) return EINVAL;
  try {
#ifdef P12_TESTING
    if (fail_field == static_cast<int>(i)) throw std::bad_alloc();
#endif
    auto& out = static_cast<std::string*>(ptr)[i];
    if (size > out.max_size()) return EOVERFLOW;
    out.assign(bytes ? bytes : "", size);
    return 0;
  } catch (const std::bad_alloc&) { return ENOMEM; }
    catch (const std::length_error&) { return EOVERFLOW; }
}
P12_EXPORT Eina_Bool p12_ewk_parse_cookie(const std::string& input, EWKCookieContents& output) {
  // ewk_cookie_parser.cc:32-34 rejects before writes; :36-48 preserves absent
  // Domain/Path; :54-77 writes expiry on every success. Never read old expiry:
  // callers may leave that scalar uninitialized before the first call.
  P12CookieState initial{{{output.domain.data(), output.domain.size()},
                         {output.name.data(), output.name.size()},
                         {output.path.data(), output.path.size()},
                         {output.value.data(), output.value.size()}}, errno};
  int saved_errno = errno;
  P12CookieResult result{0, saved_errno};
  int rc;
  try {
    // Only backend constructs EWKCookieContents within this DSO, avoiding
    // same-name implicit destructors for incompatible field layouts.
    std::string next[4];
    rc = p12_cookie_parse({input.data(), input.size()}, &initial, next, emit, &result);
    if (rc == 1) {
      // Retain storage for unchanged fields, including absent Domain/Path.
      // All allocations precede the no-throw commit phase.
      if (output.domain != next[0]) output.domain.swap(next[0]);
      if (output.name != next[1]) output.name.swap(next[1]);
      if (output.path != next[2]) output.path.swap(next[2]);
      if (output.value != next[3]) output.value.swap(next[3]);
      output.expiry_date_utc = static_cast<time_t>(result.expiry);
    }
  } catch (const std::bad_alloc&) { rc = -ENOMEM; }
    catch (const std::length_error&) { rc = -EOVERFLOW; }
  errno = rc < 0 ? -rc : result.provider_errno;
#ifdef P12_TESTING
  std::fprintf(stderr, "P12_ADAPTOR_ENTRY result=%d errno=%d string_size=%zu\n", rc, errno, sizeof(std::string));
#endif
  return rc == 1 ? EINA_TRUE : EINA_FALSE;
}
#ifndef P12_EMBEDDED
P12_EXPORT Eina_Bool ewk_parse_cookie(const std::string&, EWKCookieContents&)
    __attribute__((alias("p12_ewk_parse_cookie")));
#endif
