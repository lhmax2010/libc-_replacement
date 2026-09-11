#pragma once
#include <stddef.h>
#include <stdint.h>
struct ByteSpan { const char* p; size_t n; };
struct CookieState { ByteSpan fields[4]; int64_t expiry; };
typedef int (*EmitState)(void*,unsigned,const char*,size_t);
extern "C" int bridge_configure(const char*);
extern "C" int bridge_parse(ByteSpan,const CookieState*,void*,EmitState,int64_t*);
