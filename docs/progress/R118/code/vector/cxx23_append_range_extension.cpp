#include "common.h"
#include <array>
#include <vector>
int main(){identity();std::array<int,2>a{5,9};std::vector<int>v{2};v.append_range(a);printf("size=%zu values=%d,%d,%d\n",v.size(),v[0],v[1],v[2]);assert(v==std::vector<int>({2,5,9}));}
