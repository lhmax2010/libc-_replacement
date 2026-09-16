#include <ewk_cookie_parser.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <dlfcn.h>
extern "C" {void trace_begin();void trace_fail_after(size_t);void trace_stop();void trace_result(size_t*,size_t*,size_t*);void trace_dump();}
int main(int argc,char**argv){
 setvbuf(stdout,nullptr,_IONBF,0);if(argc!=5)return 20;
 void*h=dlopen(argv[1],RTLD_NOW|RTLD_LOCAL);if(!h){puts(dlerror());return 21;}
 auto call=reinterpret_cast<Eina_Bool(*)(const std::string&,EWKCookieContents&)>(dlsym(h,"ewk_parse_cookie"));if(!call)return 22;
 auto initial=[](){EWKCookieContents c{};c.name="old";c.value="oldvalue";c.domain="keep";c.path="/keep";c.expiry_date_utc=123;return c;};
 auto warm=initial();if(!call("warm=yes; Domain=warm; Path=/warm",warm))return 23;
 FILE*f=fopen("/proc/self/maps","r");if(!f)return 24;char line[4096];bool g=false,c=false,p=false;
 while(fgets(line,sizeof line,f)){fputs(line,stdout);g|=strstr(line,"/libstdc++.so.")!=nullptr;c|=strstr(line,"/libc++.so.")!=nullptr;p|=strstr(line,argv[1])!=nullptr;}fclose(f);
 printf("MAP_ASSERT gnu=%d cxx=%d provider=%d\n",g,c,p);if(!g||c||!p)return 25;
 std::string name(256,'N'),value(512,'V'),input=name+"="+value+"; Domain=new.example; Path=/new";
 size_t nth=strtoul(argv[2],nullptr,10);int repeats=atoi(argv[3]),want_clean=atoi(argv[4]);
 int caught=0,success=0;bool unchanged=true,exact=true;
 trace_begin();
 for(int i=0;i<repeats;++i){
  auto out=initial();trace_fail_after(nth);
  try {bool ok=call(input,out);success+=ok;exact&=ok&&out.name==name&&out.value==value&&out.domain=="new.example"&&out.path=="/new"&&out.expiry_date_utc==0;}
  catch(const std::bad_alloc&){++caught;unchanged&=out.name=="old"&&out.value=="oldvalue"&&out.domain=="keep"&&out.path=="/keep"&&out.expiry_date_utc==123;}
  catch(...){trace_stop();return 26;}
 }
 trace_stop();size_t live=0,bytes=0,failures=0;trace_result(&live,&bytes,&failures);trace_dump();
 printf("SUMMARY nth=%zu repeats=%d caught=%d success=%d failures=%zu live=%zu bytes=%zu unchanged=%d exact=%d\n",nth,repeats,caught,success,failures,live,bytes,unchanged,exact);
 if(nth&&caught!=repeats)return 27;
 if(!nth&&(success!=repeats||!exact||live||bytes))return 28;
 if(want_clean&&(live||bytes))return 29;
 return 0;
}
