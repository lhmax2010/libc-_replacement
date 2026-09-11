#pragma once
#include <stddef.h>
typedef int (*api0911_sink)(void*, const char*, size_t);
extern "C" __attribute__((visibility("hidden"))) int api0911_convert(
    const wchar_t*, size_t, api0911_sink, void*);
