#pragma once
#include <stddef.h>
#include <stdint.h>
// 仅 POD、数据指针和长度；context 只由接收侧解释。
typedef int (*ewk_emit)(void*,unsigned,const char*,size_t);
#ifdef __cplusplus
extern "C" {
#endif
__attribute__((visibility("hidden"))) int ewk_gnu_configure(const char*);
__attribute__((visibility("hidden"))) int ewk_gnu_parse(const char*,size_t,void*,ewk_emit,int64_t*);
#ifdef __cplusplus
}
#endif
