#include "common.h"
#include "fail_new.h"
#include <functional>
void* operator new(std::size_t n,std::align_val_t a){if(fail_new)throw std::bad_alloc();void*p=nullptr;std::size_t align=static_cast<std::size_t>(a);if(align<sizeof(void*))align=sizeof(void*);if(posix_memalign(&p,align,n?n:1))throw std::bad_alloc();++new_calls;return p;}
void operator delete(void*p,std::align_val_t)noexcept{::operator delete(p);}
void operator delete(void*p,std::size_t,std::align_val_t)noexcept{::operator delete(p);}
template<int N,int A=1>struct alignas(A) Trivial{char bytes[N]{};int operator()()const{return 71;}};
template<bool NT>struct Nontrivial{int x=73;Nontrivial()=default;Nontrivial(const Nontrivial&o)noexcept(NT):x(o.x){}~Nontrivial(){}int operator()()const{return x;}};
template<class T>void measure(const char*label,int expected){long before=new_calls,freed=delete_calls;long alloc;int value;{T t;std::function<int()>f=t;alloc=new_calls-before;value=f();assert(value==expected);printf("target=%s size=%zu align=%zu allocations=%ld value=%d\n",label,sizeof(T),alignof(T),alloc,value);}printf("target=%s frees=%ld\n",label,delete_calls-freed);assert(new_calls-before==delete_calls-freed);}
int main(){identity();measure<Trivial<1>>("trivial1",71);measure<Trivial<4>>("trivial4",71);measure<Trivial<8>>("trivial8",71);measure<Trivial<9>>("trivial9",71);measure<Trivial<12>>("trivial12",71);measure<Trivial<16>>("trivial16",71);measure<Trivial<17>>("trivial17",71);measure<Trivial<24>>("trivial24",71);measure<Trivial<32>>("trivial32",71);measure<Trivial<8,8>>("align8",71);measure<Trivial<8,16>>("align16",71);measure<Trivial<8,32>>("align32",71);measure<Nontrivial<true>>("nontrivial_nothrow_copy",73);measure<Nontrivial<false>>("nontrivial_throwing_copy",73);}
