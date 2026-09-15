#include <ewk_cookie_parser.h>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <dlfcn.h>
static void hex(const char* tag,const std::string& s) {
  printf(" %s=",tag); for(unsigned char c:s)printf("%02x",c);
}
int main(int argc,char**argv) {
  if(argc!=3)return 20;setvbuf(stdout,nullptr,_IONBF,0);
  if(!dlopen("libc++.so.1",RTLD_NOW|RTLD_GLOBAL)||!dlopen("libstdc++.so.6",RTLD_NOW|RTLD_GLOBAL))return 21;
  void* h=dlopen(argv[1],RTLD_NOW|RTLD_LOCAL);
  if(!h||!dlopen(argv[2],RTLD_NOW|RTLD_LOCAL)){puts(dlerror());return 22;}
  auto call=reinterpret_cast<Eina_Bool(*)(const std::string&,EWKCookieContents&)>(dlsym(h,"ewk_parse_cookie"));
  if(!call)return 23;
  FILE* f=fopen("/proc/self/maps","r");char line[4096];bool g=false,c=false,e=false,p=false;
  if(!f)return 24;while(fgets(line,sizeof line,f)){fputs(line,stdout);g|=strstr(line,"/libstdc++.so.")!=nullptr;c|=strstr(line,"/libc++.so.")!=nullptr;e|=strstr(line,argv[1])!=nullptr;p|=strstr(line,argv[2])!=nullptr;}fclose(f);
  if(!g||!c||!e||!p)return 25;puts("MAP_ASSERT gnu=1 cxx=1 entry=1 provider=1");
  for(int k=0;k<17;++k) {
    EWKCookieContents out{};out.domain="keep";out.name="old";out.path="/keep";out.value="value";out.expiry_date_utc=999;
    std::string input="sid=abc";
    switch(k) {
      case 0:input=std::string("sid=ab\0cd",9);break;
      case 1:input="sid="+std::string("\xff\x80",2);break;
      case 2:input+="; Path=";break;
      case 3:input+="; Domain=first; Domain=second; Path=/a; Path=/b";break;
      case 4:input+="; Max-Age=0";break;
      case 5:input+="; Max-Age=bad; Expires=Wed, 21 Oct 2015 07:28:00 GMT";break;
      case 6:input+="; Max-Age=-1";break;
      case 7:input+="; Max-Age=+60";break;
      case 8:input+="; Max-Age=60x";break;
      case 9:input+="; Max-Age="+std::string(128,'9');break;
      case 10:input="sid="+std::string(65536,'x');break;
      case 11:input="sid="+std::string(4092,'x');break;
      case 12:input="sid="+std::string(4093,'x');break;
      case 13:if(!call("first=one; Domain=chain; Path=/chain",out))return 26;input+="; HttpOnly";break;
      case 14:if(call("first=one; Secure",out))return 26;break;
      case 15:input+="; Domain="+std::string("\xe4\xb8\xad",3)+".example";break;
      case 16:out.value="aliased=original; Domain=alias.example";break;
    }
    time_t before=time(nullptr);bool valid=k==16?call(out.value,out):call(input,out);time_t after=time(nullptr);
    printf("BOUNDARY=%d VALID=%d",k,valid);hex("domain",out.domain);hex("name",out.name);hex("path",out.path);hex("value",out.value);
    printf(" expiry=%lld before=%lld after=%lld\n",(long long)out.expiry_date_utc,(long long)before,(long long)after);
  }
  puts("BOUNDARY_COMPLETE=17 DESTROYED=17");return 0;
}
