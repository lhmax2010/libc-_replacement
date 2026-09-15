#include "bridge.h"
#include <vector>
#include <cstdio>
#include <cstring>
#include <dlfcn.h>
static int maps(const char* entry, const char* provider) {
  FILE* f = std::fopen("/proc/self/maps", "r"); if (!f) return 0;
  char line[4096]; bool gnu=false, cxx=false, e=false, p=false;
  while (std::fgets(line, sizeof line, f)) {
    std::fputs(line, stdout);
    gnu |= std::strstr(line, "/libstdc++.so.") != nullptr;
    cxx |= std::strstr(line, "/libc++.so.") != nullptr;
    e |= std::strstr(line, entry) != nullptr; p |= std::strstr(line, provider) != nullptr;
  }
  std::fclose(f); std::printf("MAP_ASSERT gnu=%d cxx=%d entry=%d provider=%d\n",gnu,cxx,e,p);
  return gnu && cxx && e && p;
}
int main(int argc, char** argv) {
  if (argc != 3) return 20;
  std::setvbuf(stdout, nullptr, _IONBF, 0);
  if (!dlopen("libc++.so.1",RTLD_NOW|RTLD_GLOBAL) || !dlopen("libstdc++.so.6",RTLD_NOW|RTLD_GLOBAL)) return 21;
  void* entry=dlopen(argv[1],RTLD_NOW|RTLD_LOCAL); void* provider=dlopen(argv[2],RTLD_NOW|RTLD_LOCAL);
  if (!entry || !provider) { std::puts(dlerror()); return 22; }
  using Send=int(*)(int32_t,const std::vector<uint8_t>&);
  auto send=reinterpret_cast<Send>(dlsym(entry,"p12_send_reply"));
  auto check=reinterpret_cast<int(*)(int32_t,const uint8_t*,size_t)>(dlsym(provider,"p12_check_reply"));
  auto reset=reinterpret_cast<size_t(*)()>(dlsym(provider,"p12_reset_reply"));
  if (!send || !check || !reset || !maps(argv[1],argv[2])) return 23;
  int passed=0, destroyed=0;
  for (int k=0;k<8;++k) {
    {
      std::vector<uint8_t> input;
      if (k==1) input={0};
      if (k==2) input={255};
      if (k==3) for (int i=0;i<256;++i) input.push_back(i);
      if (k==4) { input.resize(65536); for(size_t i=0;i<input.size();++i) input[i]=(i*17+3)&255; }
      if (k==5) { input.reserve(4096); input={0,255,1,128,0,127}; }
      if (k==6) { input.resize(4096,63); input.resize(3); input[1]=0; }
      if (k==7) { input.resize(1024,255); input.clear(); }
      const auto expected=input;
      int32_t instance=100+k;
      int status=send(instance,input);
      // Source lifetime ends before checking the receiver's retained local copy.
      std::vector<uint8_t>().swap(input);
      bool exact=status==1 && check(instance,expected.data(),expected.size());
      bool cleared=reset()==0;
      std::printf("CASE=%d INSTANCE=%d LENGTH=%zu EXACT=%d RESET=%d\n",k,instance,expected.size(),exact,cleared);
      passed += exact && cleared;
    }
    ++destroyed;
  }
  std::printf("SUMMARY EXACT=%d/8 DESTROYED=%d/8\n",passed,destroyed);
  return passed==8 && destroyed==8 ? 0 : 28;
}
