#pragma once
#include <cstdlib>
#include <new>
// 单线程限定：只在被测操作周围开启；统计本程序可替换 new，非全进程泄漏证明。
static bool fail_new=false;
static long new_calls=0, delete_calls=0;
void* operator new(std::size_t n) {
  if (fail_new) throw std::bad_alloc();
  void* p=std::malloc(n?n:1); if(!p) throw std::bad_alloc();
  ++new_calls; return p;
}
void operator delete(void* p) noexcept { if(p) ++delete_calls; std::free(p); }
void operator delete(void* p,std::size_t) noexcept { ::operator delete(p); }
void* operator new[](std::size_t n) { return ::operator new(n); }
void operator delete[](void* p) noexcept { ::operator delete(p); }
void operator delete[](void* p,std::size_t) noexcept { ::operator delete(p); }
