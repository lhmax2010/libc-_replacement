#include <ewk_cookie_parser.h>
#include "p13_alloc_report.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <exception>
#include <new>
#include <dlfcn.h>
static EWKCookieContents initial(){EWKCookieContents o{};o.domain="keep";o.name="old";o.path="/keep";o.value="oldvalue";o.expiry_date_utc=123;return o;}
static bool equal(const EWKCookieContents&a,const EWKCookieContents&b){return a.domain==b.domain&&a.name==b.name&&a.path==b.path&&a.value==b.value&&a.expiry_date_utc==b.expiry_date_utc;}
int main(int argc,char**argv){
 if(argc!=5)return 20;setvbuf(stdout,nullptr,_IONBF,0);
 int original=atoi(argv[3]);size_t nth=strtoul(argv[4],nullptr,10);
 void*h=dlopen(argv[1],RTLD_NOW|RTLD_LOCAL);if(!h||!dlopen(argv[2],RTLD_NOW|RTLD_LOCAL))return 21;
 auto call=reinterpret_cast<Eina_Bool(*)(const std::string&,EWKCookieContents&)>(dlsym(h,"ewk_parse_cookie"));if(!call)return 22;
 auto begin=reinterpret_cast<void(*)()>(dlsym(RTLD_DEFAULT,"p13_alloc_begin"));auto end=reinterpret_cast<void(*)(P13AllocReport*)>(dlsym(RTLD_DEFAULT,"p13_alloc_end"));
 auto fail=reinterpret_cast<void(*)(size_t)>(dlsym(RTLD_DEFAULT,"p13_alloc_fail_on"));auto failed=reinterpret_cast<size_t(*)()>(dlsym(RTLD_DEFAULT,"p13_alloc_failed"));if(!begin||!end||!fail||!failed)return 23;
 auto fail_after=reinterpret_cast<void(*)(size_t)>(dlsym(RTLD_DEFAULT,"p13_alloc_fail_after"));if(!fail_after)return 23;
 auto warm=initial();if(!call("warm=yes; Domain=warm; Path=/warm",warm))return 24;
 FILE*f=fopen("/proc/self/maps","r");if(!f)return 25;char line[4096];bool g=false,c=false,e=false,p=false;
 while(fgets(line,sizeof line,f)){fputs(line,stdout);g|=strstr(line,"/libstdc++.so.")!=nullptr;c|=strstr(line,"/libc++.so.")!=nullptr;e|=strstr(line,argv[1])!=nullptr;p|=strstr(line,argv[2])!=nullptr;}fclose(f);
 printf("MAP_ASSERT gnu=%d cxx=%d entry=%d provider=%d original=%d\n",g,c,e,p,original);if(!g||!e||!p||(!original&&!c)||(original&&c))return 26;
 auto out=initial();auto before=out;std::string name(256,'N'),value(512,'V');std::string input=name+"="+value+"; Domain=new.example; Path=/new";
 if(nth==999)input="";
 bool returned=false,valid=false;int caught=0;P13AllocReport allocation{};
 begin();fail(nth==999?0:nth);errno=17;
 int repetitions=nth==1004?10:1;
 for(int i=0;i<repetitions;++i){
  if(nth==1004)fail_after(4);
  try{valid=call(input,out);returned=true;}catch(const std::bad_alloc&){caught=1;}catch(const std::exception&){caught=2;}catch(...){caught=3;}
 }
 int error=errno;size_t injected=failed();end(&allocation);
 bool unchanged=equal(out,before);bool exact=out.name==name&&out.value==value&&out.domain=="new.example"&&out.path=="/new"&&out.expiry_date_utc==0;
 printf("CONTRACT original=%d nth=%zu injected=%zu returned=%d valid=%d caught=%d errno=%d unchanged=%d success_exact=%d allocations=%zu frees=%zu live=%zu bytes=%zu\n",original,nth,injected,returned,valid,caught,error,unchanged,exact,allocation.allocations,allocation.frees,allocation.live,allocation.bytes);
 printf("FIELDS name_length=%zu value_length=%zu domain=%s path=%s expiry=%lld\n",out.name.size(),out.value.size(),out.domain.c_str(),out.path.c_str(),(long long)out.expiry_date_utc);
 if(nth==1004)printf("REPEATED_FAILURES=%d tracked_outstanding=%zu tracked_bytes=%zu\n",repetitions,allocation.live,allocation.bytes);
 if(nth==0&&(!returned||!valid||!exact))return 27;
 if(nth==999&&(!returned||valid||!unchanged))return 28;
 if(!original&&injected&&(!returned||valid||caught||error!=ENOMEM||!unchanged||allocation.live||allocation.bytes))return 29;
 return 0;
}
