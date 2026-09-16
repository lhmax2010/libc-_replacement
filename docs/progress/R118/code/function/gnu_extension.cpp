#include "common.h"
#include "fail_new.h"
#include <functional>
// 纯整数状态，无自引用：该 GNU 扩展允许声明对象可重定位；并非任意类型都可这样声明。
struct Relocatable{int value=37;~Relocatable(){}int operator()()const{return value;}};
namespace std {template<>struct __is_location_invariant<Relocatable>:true_type{};}
int main(){identity();long old=new_calls;{std::function<int()>f=Relocatable{};printf("value=%d allocations=%ld\n",f(),new_calls-old);assert(f()==37);} }
