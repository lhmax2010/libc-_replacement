#include "common.h"
#include <vector>
int main(){identity();int a[]={3,5,8};std::vector<int> empty,range(a,a+3),copy=range,moved=std::move(copy),assigned;assigned=range;std::vector<int> ma;ma=std::move(assigned);
 printf("empty_size=%zu moved_from_size=%zu values=",empty.size(),copy.size());for(int n:moved)printf("%d,",n);printf(" move_assigned_size=%zu\n",ma.size());assert(empty.empty()&&moved==range&&ma==range);
 std::vector<std::string> nested={"a",std::string("b\0c",3)};printf("nested_sizes=%zu,%zu nested_byte2=%d\n",nested[0].size(),nested[1].size(),int(nested[1][2]));assert(nested[1][2]=='c');
}
