// 正常但不含 Domain/Path 的输入；核查输出对象复用时的状态语义。
#include <ewk_cookie_parser.h>
#include <cstdio>
#include <cstring>
#include <dlfcn.h>
int main(int argc,char**argv){
 if(argc!=3)return 20;setvbuf(stdout,nullptr,_IONBF,0);
 if(!dlopen("libc++.so.1",RTLD_NOW|RTLD_GLOBAL)||!dlopen("libstdc++.so.6",RTLD_NOW|RTLD_GLOBAL))return 21;
 void*h=dlopen(argv[1],RTLD_NOW|RTLD_LOCAL);if(!h)return 22;
 if(strcmp(argv[1],argv[2])){auto init=reinterpret_cast<int(*)(const char*)>(dlsym(h,"api_probe_configure_real_provider"));if(!init||!init(argv[2]))return 23;}
 auto call=reinterpret_cast<Eina_Bool(*)(const std::string&,EWKCookieContents&)>(dlsym(h,"ewk_parse_cookie"));if(!call)return 24;
 FILE*f=fopen("/proc/self/maps","r");if(!f)return 25;char line[4096];bool cxx=0,gnu=0,entry=0,provider=0;
 while(fgets(line,sizeof line,f)){fputs(line,stdout);cxx|=strstr(line,"/libc++.so.")!=nullptr;gnu|=strstr(line,"/libstdc++.so.")!=nullptr;entry|=strstr(line,argv[1])!=nullptr;provider|=strstr(line,argv[2])!=nullptr;}fclose(f);
 printf("MAP_ASSERT cxx=%d gnu=%d entry=%d provider=%d\n",cxx,gnu,entry,provider);if(!cxx||!gnu||!entry||!provider)return 26;
 bool exact;
 {
  std::string input="sid=ABC123";EWKCookieContents output{};output.domain="keep.example";output.name="oldname";output.path="/keep";output.value="oldvalue";output.expiry_date_utc=43;
  bool valid=call(input,output);
  printf("REUSED_OUTPUT valid=%d domain=%s name=%s path=%s value=%s expiry=%lld\n",valid,output.domain.c_str(),output.name.c_str(),output.path.c_str(),output.value.c_str(),(long long)output.expiry_date_utc);
  exact=valid&&output.domain=="keep.example"&&output.name=="sid"&&output.path=="/keep"&&output.value=="ABC123"&&output.expiry_date_utc==0;
  printf("REUSED_OUTPUT_MATCHES_DIRECT=%d\n",exact);
 }
 puts("REUSED_OUTPUT_DESTROYED=1");return exact?0:28;
}
