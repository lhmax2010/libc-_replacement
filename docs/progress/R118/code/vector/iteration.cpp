#include "common.h"
#include <vector>
int main(){identity();std::vector<int>v={1,2,3};v.reserve(10);auto first=v.begin();auto addr=reinterpret_cast<std::uintptr_t>(v.data());v.push_back(4);
 printf("no_realloc_first=%d address_same=%d\n",*first,int(addr==reinterpret_cast<std::uintptr_t>(v.data())));assert(*first==1);
 auto ret=v.erase(v.begin()+1);printf("erase_return=%d values=",*ret);for(int n:v)printf("%d,",n);printf("\n");assert(*ret==3&&v==std::vector<int>({1,3,4}));
 auto cap=v.capacity();v.reserve(cap+10);printf("realloc_address_same=%d contiguous_stride=%td\n",int(addr==reinterpret_cast<std::uintptr_t>(v.data())),&v[1]-&v[0]);assert(v[2]==4);
 // erase 后仅使用返回的新迭代器；重新分配后不读旧迭代器。
}
