#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>
#include <unistd.h>
extern "C" void r116_identity() {
  const char* expected=getenv("R116_PROVIDER");
  if(!expected)_Exit(90);
  FILE* f=fopen("/proc/self/maps","r"); if(!f)_Exit(91);
  char line[2048]; bool exp=false,gnu=false;
  while(fgets(line,sizeof line,f)) {
    if(strstr(line,"libunwind.so") || strstr(line,"libgcc_s.so")) {
      fputs(line,stdout);
      if(strstr(line,"/tmp/R116/install-native/"))exp=true;
      else gnu=true;
    }
  }
  fclose(f);
  Dl_info d{};void* sym=dlvsym(RTLD_DEFAULT,"_Unwind_ForcedUnwind","GCC_3.0");
  if(!sym || !dladdr(sym,&d))_Exit(92);
  bool is_exp=strstr(d.dli_fname,"/tmp/R116/install-native/")!=nullptr;
  printf("unwind_provider=%s expected=%s experimental_mapping=%d other_unwinder_mapping=%d\n",d.dli_fname,expected,exp,gnu);
  bool want=!strcmp(expected,"llvm");
  if(want?(!exp||gnu||!is_exp):(exp||!gnu||is_exp))_Exit(93);
  puts("PROVIDER_ASSERT=PASS");
}
__attribute__((constructor(101))) static void init_identity() {
  setvbuf(stdout,nullptr,_IONBF,0); alarm(15); r116_identity();
}
