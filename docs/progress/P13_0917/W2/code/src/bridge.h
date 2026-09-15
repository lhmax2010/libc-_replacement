#pragma once
#include <stddef.h>
#include <stdint.h>
typedef struct P12Span { const char* data; size_t size; } P12Span;
// Borrowed spans: null only for size zero; other pointers must be readable.
typedef struct P12CookieState { P12Span fields[4]; int caller_errno; } P12CookieState;
typedef struct P12CookieResult { int64_t expiry; int provider_errno; } P12CookieResult;
typedef int (*P12Emit)(void*, unsigned, const char*, size_t);
// Result: 1 accepted, 0 rejected, negative errno. Emit: 0 success, positive errno.
#ifdef __cplusplus
#define P12_C extern "C"
#else
#define P12_C extern
#endif
P12_C int p12_cookie_parse(P12Span, const P12CookieState*, void*, P12Emit, P12CookieResult*);
P12_C int p12_vector_bytes(int32_t, const uint8_t*, size_t);
#define P12_EXPORT P12_C __attribute__((visibility("default")))
