#include "common.h"
#include <functional>
int twice(int n){return n*2;}
int main(){identity();std::function<int(int)>f=twice;std::function<void()>v;
 printf("function_int sizeof=%zu alignof=%zu function_void_size=%zu function_void_align=%zu\n",sizeof(f),alignof(decltype(f)),sizeof(v),alignof(decltype(v)));
 printf("target_pointer_present=%d value=%d empty=%d\n",f.target<int(*)(int)>()!=nullptr,f(21),!v);assert(f(21)==42&&!v);}
