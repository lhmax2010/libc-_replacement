#include "bridge.h"
#include "provider_loader.h"
#include <ewk_cookie_parser.h>
#include <cerrno>
#include <limits>
#include <new>
#include <stdexcept>
#ifndef P12_PROVIDER_PATH
#define P12_PROVIDER_PATH "/usr/share/chromium-efl/lib/libchromium-impl.so"
#endif
namespace {
p12_loader::State provider;
thread_local bool opening = false;
void initialize() { provider.error = p12_loader::resolve(provider, P12_PROVIDER_PATH, "ewk_parse_cookie"); }
int ready() {
  if (opening) return EDEADLK;
  opening = true;
  struct Reset { ~Reset() { opening = false; } } reset;
  int rc = pthread_once(&provider.once, initialize);
  return rc ? rc : provider.error;
}
}
extern "C" int p12_cookie_parse(P12Span input, const P12CookieState* initial,
                                void* ctx, P12Emit emit, P12CookieResult* result) {
  if (!initial || !ctx || !emit || !result || (!input.data && input.size)) return -EINVAL;
  try {
    const size_t limit = std::string().max_size();
    if (input.size > limit) return -EOVERFLOW;
    size_t total = input.size;
    for (unsigned i = 0; i < 4; ++i) {
      P12Span s = initial->fields[i];
      if (!s.data && s.size) return -EINVAL;
      if (s.size > limit || s.size > std::numeric_limits<size_t>::max() - total) return -EOVERFLOW;
      total += s.size;
    }
    int error = ready(); if (error) return -error;
    using Fn = Eina_Bool (*)(const std::string&, EWKCookieContents&);
    auto real = reinterpret_cast<Fn>(provider.symbol);
    std::string text(input.data ? input.data : "", input.size);
    EWKCookieContents output{};
    std::string* fields[] = {&output.domain, &output.name, &output.path, &output.value};
    // Source :39-48: optional attributes overwrite only when present.
    for (unsigned i = 0; i < 4; ++i)
      fields[i]->assign(initial->fields[i].data ? initial->fields[i].data : "", initial->fields[i].size);
    // Source :32: pass all bytes, not strlen/transcoding; embedded NUL and
    // non-UTF-8 interpretation belongs to the real net::ParsedCookie.
    // Source :59 strtol may set ERANGE; do not silently erase provider errno.
    errno = initial->caller_errno;
    bool accepted = real(text, output);
    result->provider_errno = errno;
    if (!accepted) return 0;
    for (unsigned i = 0; i < 4; ++i) {
      int status = emit(ctx, i, fields[i]->data(), fields[i]->size());
      if (status) return -status;
    }
    result->expiry = output.expiry_date_utc;
    return 1;
  } catch (const std::bad_alloc&) { return -ENOMEM; }
    catch (const std::length_error&) { return -EOVERFLOW; }
  // No catch-all: never swallow foreign/forced unwind. Other provider
  // exceptions remain outside the approved contract; no safety claim.
}
