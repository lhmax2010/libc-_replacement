#include <ewk_cookie_parser.h>
#include <cstdio>
#include <cstring>
#include <dlfcn.h>
int main(int argc,char**argv){
 if(argc!=3)return 20;setvbuf(stdout,nullptr,_IONBF,0);
 if(!dlopen("libc++.so.1",RTLD_NOW|RTLD_GLOBAL)||!dlopen("libstdc++.so.6",RTLD_NOW|RTLD_GLOBAL))return 21;
 void*h=dlopen(argv[1],RTLD_NOW|RTLD_LOCAL);if(!h){printf("SHIM_LOAD=%s\n",dlerror());return 22;}
 auto configure=reinterpret_cast<int(*)(const char*)>(dlsym(h,"api_probe_configure_real_provider"));
 if(!configure||!configure(argv[2]))return 23;
 auto call=reinterpret_cast<Eina_Bool(*)(const std::string&,EWKCookieContents&)>(dlsym(h,"ewk_parse_cookie"));if(!call)return 24;
 FILE*f=fopen("/proc/self/maps","r");if(!f)return 25;char line[4096];bool cxx=0,gnu=0,shim=0,provider=0;
 while(fgets(line,sizeof line,f)){fputs(line,stdout);cxx|=strstr(line,"/libc++.so.")!=nullptr;gnu|=strstr(line,"/libstdc++.so.")!=nullptr;shim|=strstr(line,argv[1])!=nullptr;provider|=strstr(line,argv[2])!=nullptr;}fclose(f);
 printf("MAP_ASSERT cxx=%d gnu=%d shim=%d provider=%d\n",cxx,gnu,shim,provider);if(!cxx||!gnu||!shim||!provider)return 26;
 {
  std::string input="sid=ABC123; Domain=example.test; Path=/probe";EWKCookieContents output{};
  bool ok=call(input,output);
  printf("VALUE domain=%s name=%s path=%s value=%s expiry=%lld\n",output.domain.c_str(),output.name.c_str(),output.path.c_str(),output.value.c_str(),(long long)output.expiry_date_utc);
  bool exact=ok&&output.domain=="example.test"&&output.name=="sid"&&output.path=="/probe"&&output.value=="ABC123"&&output.expiry_date_utc==0;
  printf("EXACT_FIELDS=%d\n",exact);if(!exact)return 27;
 }
 puts("SHIM_CONSUMER_DESTROYED=1");return 0;
}
