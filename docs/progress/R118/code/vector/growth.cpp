#include "common.h"
#include <vector>
int main(){identity();std::vector<int>v;std::size_t cap=0;printf("size=0 capacity=0\n");for(int i=0;i<130;++i){v.push_back(i);if(v.capacity()!=cap){cap=v.capacity();printf("size=%zu capacity=%zu\n",v.size(),cap);}assert(v.back()==i);}
 for(std::size_t n:{0,1,7,16,23,100}){std::vector<int>x={1,2,3};x.reserve(n);printf("reserve=%zu size=%zu capacity=%zu\n",n,x.size(),x.capacity());assert(x.size()==3&&x[1]==2&&x.capacity()>=n);}
 std::vector<int>x(100,9);x.resize(3);auto old=x.capacity();x.shrink_to_fit();printf("shrink before=%zu after=%zu size=%zu value=%d\n",old,x.capacity(),x.size(),x[0]);assert(x==std::vector<int>(3,9));
}
