#include <boost/program_options/detail/convert.hpp>
#include <cstdio>
#include <cstring>
#include <dlfcn.h>
#include <string>
#include <vector>
int main(int argc,char**argv) {
  if(argc!=3) return 60;
  // 同侧控制也显式加载另一运行库，避免把“未共存”当混合场景的控制。
  if(!dlopen("libc++.so.1",RTLD_NOW|RTLD_GLOBAL)||!dlopen("libstdc++.so.6",RTLD_NOW|RTLD_GLOBAL)) {
    std::fprintf(stderr,"dlopen=%s\n",dlerror());return 61;
  }
  const std::wstring unicode=L"A\u4e2d\u6587\U0001F642";
  const std::string unicode_bytes="A\xe4\xb8\xad\xe6\x96\x87\xf0\x9f\x99\x82";
  const std::wstring cases[]={L"",L"ASCII-123",unicode,std::wstring(L"a\0b",3),std::wstring(257,L'x')};
  const std::string expected[]={"","ASCII-123",unicode_bytes,std::string("a\0b",3),std::string(257,'x')};
  bool values=true;
  for(size_t i=0;i<5;++i) {
    { const auto answer=boost::program_options::to_internal(cases[i]);
      bool ok=answer==expected[i]; values&=ok;
      std::printf("VALUE case=%zu bytes=%zu expected=%zu exact=%d hex=",i,answer.size(),expected[i].size(),ok);
      for(unsigned char c:answer) std::printf("%02x",c);
      std::puts("");
    }
    std::printf("CALLER_OUTPUT_DESTROYED case=%zu\n",i);
  }
  if(std::strcmp(argv[2],"shim")==0) {
    auto done=reinterpret_cast<int(*)()>(dlsym(RTLD_DEFAULT,"api0911_completed"));
    if(!done || done()!=5)return 62;
    std::printf("PROVIDER_SCOPES_COMPLETED=%d\n",done());
  }
  bool cxx=false,gnu=false,provider=false;
  FILE* f=std::fopen("/proc/self/maps","r");if(!f)return 63;
  char line[4096];
  while(std::fgets(line,sizeof line,f)) {
    std::fputs(line,stdout);
    cxx|=std::strstr(line,"/libc++.so.")!=nullptr;
    gnu|=std::strstr(line,"/libstdc++.so.")!=nullptr;
    provider|=std::strstr(line,argv[1])!=nullptr;
  }
  std::fclose(f);
  std::printf("ASSERT values=%d maps_cxx=%d maps_gnu=%d maps_provider=%d sizeof_string=%zu sizeof_vector=%zu\n",values,cxx,gnu,provider,sizeof(std::string),sizeof(std::vector<unsigned char>));
  return values&&cxx&&gnu&&provider ? 0:64;
}
