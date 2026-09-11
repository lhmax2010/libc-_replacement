#include <ewk_cookie_parser.h>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <dlfcn.h>
static void hex(const char*label,const std::string&s){printf(" %s[%zu]=",label,s.size());for(unsigned char c:s)printf("%02x",c);}
static EWKCookieContents initial(){EWKCookieContents o{};o.domain="keep.example";o.name="oldname";o.path="/keep";o.value="oldvalue";o.expiry_date_utc=43;return o;}
int main(int argc,char**argv){
 if(argc!=3)return 20;setvbuf(stdout,nullptr,_IONBF,0);
 if(!dlopen("libc++.so.1",RTLD_NOW|RTLD_GLOBAL)||!dlopen("libstdc++.so.6",RTLD_NOW|RTLD_GLOBAL))return 21;
 void*h=dlopen(argv[1],RTLD_NOW|RTLD_LOCAL);if(!h){puts(dlerror());return 22;}
 if(strcmp(argv[1],argv[2])){auto init=reinterpret_cast<int(*)(const char*)>(dlsym(h,"api_probe_configure_real_provider"));if(!init||!init(argv[2]))return 23;}
 auto call=reinterpret_cast<Eina_Bool(*)(const std::string&,EWKCookieContents&)>(dlsym(h,"ewk_parse_cookie"));if(!call)return 24;
 FILE*f=fopen("/proc/self/maps","r");if(!f)return 25;char line[4096];bool cxx=0,gnu=0,entry=0,provider=0;
 while(fgets(line,sizeof line,f)){fputs(line,stdout);cxx|=strstr(line,"/libc++.so.")!=nullptr;gnu|=strstr(line,"/libstdc++.so.")!=nullptr;entry|=strstr(line,argv[1])!=nullptr;provider|=strstr(line,argv[2])!=nullptr;}fclose(f);
 printf("MAP_ASSERT cxx=%d gnu=%d entry=%d provider=%d\n",cxx,gnu,entry,provider);if(!cxx||!gnu||!entry||!provider)return 26;
 int passed=0,destroyed=0;
 for(int k=0;k<12;k++){
  {
   auto out=initial();auto want=out;std::string input="sid=ABC123";bool valid_expected=true;bool setup=true;
   want.name="sid";want.value="ABC123";want.expiry_date_utc=0;
   switch(k){
    case 0:out=EWKCookieContents{};input+="; Domain=example.com; Path=/new";want.domain="example.com";want.path="/new";break;
    case 1:break;
    case 2:input+="; Domain=new.example";want.domain="new.example";break;
    case 3:input+="; Path=/new";want.path="/new";break;
    case 4:input+="; Domain=";want.domain="";break;
    case 5:input="";valid_expected=false;want=out;break;
    case 6:input+="; HttpOnly";valid_expected=false;want=out;break;
    case 7:input+="; Secure";valid_expected=false;want=out;break;
    case 8:out.domain=std::string(160,'d');out.path="/"+std::string(160,'p');input="sid="+std::string(1024,'V');want.domain=out.domain;want.path=out.path;want.value=std::string(1024,'V');break;
    case 9:input+="; Expires=Wed, 21 Oct 2015 07:28:00 GMT";want.expiry_date_utc=1445412480;break;
    case 10:input+="; Max-Age=60";break;
    case 11:{
      setup=call("first=one; Domain=chain.example; Path=/chain",out);
      setup=setup&&out.name=="first"&&out.value=="one"&&out.domain=="chain.example"&&out.path=="/chain"&&out.expiry_date_utc==0;
      want.domain="chain.example";want.path="/chain";break;
    }
   }
   time_t before=time(nullptr);bool valid=call(input,out);time_t after=time(nullptr);
   bool expiry=k==10?(out.expiry_date_utc>=before+60&&out.expiry_date_utc<=after+60):out.expiry_date_utc==want.expiry_date_utc;
   bool exact=setup&&valid==valid_expected&&out.domain==want.domain&&out.name==want.name&&out.path==want.path&&out.value==want.value&&expiry;
   printf("CASE=%d VALID=%d EXACT=%d",k,valid,exact);hex("domain",out.domain);hex("name",out.name);hex("path",out.path);hex("value",out.value);printf(" expiry=%lld before=%lld after=%lld\n",(long long)out.expiry_date_utc,(long long)before,(long long)after);
   passed+=exact;
  }
  destroyed++;
 }
 printf("SUMMARY EXACT=%d/12 DESTROYED=%d/12\n",passed,destroyed);return passed==12&&destroyed==12?0:28;
}
