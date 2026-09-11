#include <ewk_cookie_parser.h>
#include <cstdio>
#include <cstring>
#include <dlfcn.h>
#include <exception>
int main(int argc,char**argv) {
  if(argc!=2)return 20;
  setvbuf(stdout,nullptr,_IONBF,0);
  if(!dlopen("libc++.so.1",RTLD_NOW|RTLD_GLOBAL) || !dlopen("libstdc++.so.6",RTLD_NOW|RTLD_GLOBAL)) {
    printf("RUNTIME_LOAD_ERROR=%s\n",dlerror());return 21;
  }
  void*lib=dlopen(argv[1],RTLD_NOW|RTLD_LOCAL);
  if(!lib){printf("PROVIDER_LOAD_ERROR=%s\n",dlerror());return 22;}
  auto call=reinterpret_cast<Eina_Bool(*)(const std::string&,EWKCookieContents&)>(dlsym(lib,"ewk_parse_cookie"));
  if(!call){puts("SYMBOL_NOT_FOUND");return 23;}
  bool cxx=false,gnu=false,provider=false;
  FILE*f=fopen("/proc/self/maps","r");if(!f)return 24;char line[4096];
  while(fgets(line,sizeof line,f)){fputs(line,stdout);cxx|=strstr(line,"/libc++.so.")!=nullptr;gnu|=strstr(line,"/libstdc++.so.")!=nullptr;provider|=strstr(line,argv[1])!=nullptr;}
  fclose(f);printf("MAP_ASSERT cxx=%d gnu=%d provider=%d\n",cxx,gnu,provider);
  if(!cxx||!gnu||!provider)return 25;
  printf("LAYOUT string=%zu output=%zu\n",sizeof(std::string),sizeof(EWKCookieContents));
  try {
    {
      std::string input="sid=ABC123; Domain=example.test; Path=/probe";
      EWKCookieContents result{};
      puts("BEFORE_REAL_CALL");
      bool ok=call(input,result);
      printf("RETURN valid=%d\n",ok);
      printf("VALUE domain=%s name=%s path=%s value=%s expiry=%lld\n",result.domain.c_str(),result.name.c_str(),result.path.c_str(),result.value.c_str(),(long long)result.expiry_date_utc);
      bool exact=ok&&result.domain=="example.test"&&result.name=="sid"&&result.path=="/probe"&&result.value=="ABC123"&&result.expiry_date_utc==0;
      printf("EXACT_FIELDS=%d\n",exact);if(!exact)return 26;
    }
    puts("CONSUMER_OBJECTS_DESTROYED=1");return 0;
  } catch(const std::exception&e){printf("EXCEPTION=%s\n",e.what());return 27;}
}
