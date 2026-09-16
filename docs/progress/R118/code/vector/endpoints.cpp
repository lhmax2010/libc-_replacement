#include "common.h"
#include <vector>
int main(){identity();std::vector<int>v={17,29,41};v.reserve(7);
 printf("size=%zu capacity=%zu\n",v.size(),v.capacity());
 std::uintptr_t wanted[]={reinterpret_cast<std::uintptr_t>(v.data()),reinterpret_cast<std::uintptr_t>(v.data()+v.size()),reinterpret_cast<std::uintptr_t>(v.data()+v.capacity())};
 for(int k=0;k<3;++k){int matches=0;printf("endpoint%d_offsets=",k);for(std::size_t off=0;off+sizeof(std::uintptr_t)<=sizeof(v);off+=alignof(void*)){std::uintptr_t word;std::memcpy(&word,reinterpret_cast<unsigned char*>(&v)+off,sizeof word);if(word==wanted[k]){printf("%zu,",off);++matches;}}printf(" matches=%d\n",matches);assert(matches==1);}
}
