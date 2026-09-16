#include "common.h"
#include <functional>
#include <vector>
int main(){identity();int n=7;std::function<int(int)>f=[n](int x){return n+x;},copy=f,moved=std::move(copy),assigned;assigned=f;std::function<int(int)>ma;ma=std::move(assigned);
 printf("values=%d,%d,%d moved_from_empty=%d\n",f(5),moved(5),ma(5),!copy);assert(f(5)==12&&moved(5)==12&&ma(5)==12);
 std::vector<std::function<int(int)>>v;v.push_back(f);printf("nested_value=%d\n",v[0](6));assert(v[0](6)==13);
 struct Counter{int n=0;int operator()(){return ++n;}} c;std::function<int()>ref=std::ref(c);int first=ref(),second=ref();printf("reference_values=%d,%d count=%d\n",first,second,c.n);assert(c.n==2);
}
