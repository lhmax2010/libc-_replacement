#include "common.h"
#include "fail_new.h"
#include <functional>
template<int N>struct F{char data[N]{};int operator()()const{return N;}};
template<int N>void check(){long before=new_calls,freed=delete_calls;{std::function<int()>f=F<N>{};printf("callable_size=%zu align=%zu allocations=%ld value=%d\n",sizeof(F<N>),alignof(F<N>),new_calls-before,f());assert(f()==N);}assert(new_calls-before==delete_calls-freed);if constexpr(N<40)check<N+1>();}
int main(){identity();check<1>();}
