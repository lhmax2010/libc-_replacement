// 辅助表示观测；不调用 SetSyncBinaryReply，不替代真实引擎的运行矩阵。
#include <vector>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <dlfcn.h>
int main(int argc,char**argv){
 if(argc!=2)return 20;setvbuf(stdout,nullptr,_IONBF,0);
 if(!dlopen("libc++.so.1",RTLD_NOW|RTLD_GLOBAL)||!dlopen("libstdc++.so.6",RTLD_NOW|RTLD_GLOBAL))return 21;
 void*h=dlopen(argv[1],RTLD_NOW|RTLD_LOCAL);if(!h){printf("LOAD_ERROR=%s\n",dlerror());return 22;}
 FILE*f=fopen("/proc/self/maps","r");if(!f)return 23;char line[4096];bool cxx=0,gnu=0,provider=0;
 while(fgets(line,sizeof line,f)){fputs(line,stdout);cxx|=strstr(line,"/libc++.so.")!=nullptr;gnu|=strstr(line,"/libstdc++.so.")!=nullptr;provider|=strstr(line,argv[1])!=nullptr;}fclose(f);
 printf("MAP_ASSERT cxx=%d gnu=%d provider=%d\n",cxx,gnu,provider);if(!cxx||!gnu||!provider)return 24;
 printf("AUXILIARY_ONLY: actual callback NOT_CALLED sizeof_vector=%zu align=%zu\n",sizeof(std::vector<uint8_t>),alignof(std::vector<uint8_t>));
 const size_t lengths[]={0,1,4,257};
 for(size_t n:lengths){
  std::vector<uint8_t>v;v.reserve(512);for(size_t i=0;i<n;++i)v.push_back(static_cast<uint8_t>((i*37+19)%256));
  if(sizeof(v)!=3*sizeof(uintptr_t))return 25;
  uintptr_t words[3];memcpy(words,&v,sizeof words);
  auto begin=reinterpret_cast<uintptr_t>(v.data());bool pointers=words[0]==begin&&words[1]==begin+v.size()&&words[2]==begin+v.capacity();
  printf("REPRESENTATION n=%zu size=%zu capacity=%zu words=%zx,%zx,%zx begin=%zx end=%zx allocation_end=%zx three_pointer_match=%d\n",n,v.size(),v.capacity(),words[0],words[1],words[2],begin,begin+v.size(),begin+v.capacity(),pointers);if(!pointers)return 26;
  bool exact=true;for(size_t i=0;i<n;i++){auto expected=static_cast<uint8_t>((i*37+19)%256);exact&=v[i]==expected;printf("BYTE index=%zu expected=%u actual=%u\n",i,unsigned(expected),unsigned(v[i]));}
  if(!exact)return 27;
  std::vector<uint8_t>copy(v);v.clear();v.shrink_to_fit();
  for(size_t i=0;i<n;i++)if(copy[i]!=static_cast<uint8_t>((i*37+19)%256))return 28;
 }
 puts("REPRESENTATION_AND_OWN_LIBRARY_COPY_DESTROY_PASS cases=4");return 0;
}
