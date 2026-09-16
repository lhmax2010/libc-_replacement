#include "common.h"
#include <vector>
#include <type_traits>
int main(){identity();std::vector<bool>b={true,false,true};auto proxy=b[1];proxy=true;b.flip();
 printf("reference_is_bool_ref=%d reference_bytes=%zu iterator_bytes=%zu bits=",int(std::is_same<decltype(b[0]),bool&>::value),sizeof(b[0]),sizeof(b.begin()));for(bool x:b)printf("%d",int(x));printf("\n");assert(!b[0]&&!b[1]&&!b[2]);
 std::size_t cap=0;for(int i=0;i<140;++i){b.push_back(i%2);if(cap!=b.capacity()){cap=b.capacity();printf("bool_size=%zu capacity=%zu\n",b.size(),cap);}}
 b.resize(3);b.shrink_to_fit();printf("bool_shrink_size=%zu capacity=%zu\n",b.size(),b.capacity());
}
