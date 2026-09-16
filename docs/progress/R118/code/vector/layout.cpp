#include "common.h"
#include <vector>
int main(){identity();std::vector<int> v={17,29,41};std::vector<bool>b={true,false,true};
 printf("vector_int sizeof=%zu alignof=%zu size=%zu capacity=%zu\n",sizeof(v),alignof(decltype(v)),v.size(),v.capacity());
 printf("vector_bool sizeof=%zu alignof=%zu size=%zu capacity=%zu proxy_size=%zu\n",sizeof(b),alignof(decltype(b)),b.size(),b.capacity(),sizeof(b[0]));
 // unsigned-char 表示读及 memcpy 合法；匹配已知 data/size/capacity 端点，不伪称字段类型由字节决定。
 std::uintptr_t wanted[]={reinterpret_cast<std::uintptr_t>(v.data()),reinterpret_cast<std::uintptr_t>(v.data()+v.size()),reinterpret_cast<std::uintptr_t>(v.data()+v.capacity())};
 for(int k=0;k<3;++k){printf("endpoint%d_offsets=",k);for(std::size_t off=0;off+sizeof(std::uintptr_t)<=sizeof(v);off+=alignof(void*)){std::uintptr_t word;std::memcpy(&word,reinterpret_cast<unsigned char*>(&v)+off,sizeof word);if(word==wanted[k])printf("%zu,",off);}printf("\n");}
 assert(v[0]==17&&v[2]==41);
}
