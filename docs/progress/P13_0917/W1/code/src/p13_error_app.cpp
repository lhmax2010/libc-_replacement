#include <ewk_cookie_parser.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <dlfcn.h>
#include <sys/resource.h>
static EWKCookieContents initial(){EWKCookieContents o{};o.domain=std::string(128,'d');o.name=std::string(128,'n');o.path=std::string(128,'p');o.value=std::string(128,'v');o.expiry_date_utc=999;return o;}
static bool equal(const EWKCookieContents&a,const EWKCookieContents&b){return a.domain==b.domain&&a.name==b.name&&a.path==b.path&&a.value==b.value&&a.expiry_date_utc==b.expiry_date_utc;}
int main(int argc,char**argv){
 if(argc!=6)return 20;setvbuf(stdout,nullptr,_IONBF,0);
 if(!dlopen("libc++.so.1",RTLD_NOW|RTLD_GLOBAL)||!dlopen("libstdc++.so.6",RTLD_NOW|RTLD_GLOBAL))return 21;
 void*h=dlopen(argv[1],RTLD_NOW|RTLD_LOCAL);if(!h||!dlopen(argv[2],RTLD_NOW|RTLD_LOCAL))return 22;
 auto call=reinterpret_cast<Eina_Bool(*)(const std::string&,EWKCookieContents&)>(dlsym(h,"ewk_parse_cookie"));
 auto configure=reinterpret_cast<void(*)(int,int)>(dlsym(h,"p13_test_configure"));
 auto counts=reinterpret_cast<void(*)(int*,int*,int*,int*)>(dlsym(h,"p13_test_counts"));
 if(!call||!configure||!counts)return 23;
 Dl_info route{};if(!dladdr(reinterpret_cast<void*>(call),&route)||strcmp(route.dli_fname,argv[1]))return 24;
 auto warm=initial();if(!call("sid=abc; Domain=warm; Path=/warm",warm))return 25;
 FILE*f=fopen("/proc/self/maps","r");if(!f)return 26;char line[4096];bool g=false,c=false,e=false,p=false;
 while(fgets(line,sizeof line,f)){fputs(line,stdout);g|=strstr(line,"/libstdc++.so.")!=nullptr;c|=strstr(line,"/libc++.so.")!=nullptr;e|=strstr(line,argv[1])!=nullptr;p|=strstr(line,argv[2])!=nullptr;}fclose(f);
 printf("MAP_ASSERT gnu=%d cxx=%d entry=%d provider=%d\n",g,c,e,p);if(!g||!c||!e||!p)return 27;
 int kind=atoi(argv[3]),phase=atoi(argv[4]),expected=atoi(argv[5]);
 auto out=initial();auto before=out;const char*storage[4]={out.domain.data(),out.name.data(),out.path.data(),out.value.data()};
 std::string input="sid=abc; Domain=new; Path=/new";
 rlimit old{};
 if(kind==10){
  // A genuinely allocated, readable input; pressure forces the backend copy
  // to fail, instead of using an invalid oversized borrowed pointer.
  input.assign(64*1024*1024,'x');
  f=fopen("/proc/self/status","r");if(!f)return 28;unsigned long vm=0;
  while(fgets(line,sizeof line,f))if(sscanf(line,"VmSize: %lu kB",&vm)==1)break;fclose(f);
  if(!vm||getrlimit(RLIMIT_AS,&old))return 29;
  rlimit cap{vm*1024+1024*1024,old.rlim_max};if(cap.rlim_cur>cap.rlim_max||setrlimit(RLIMIT_AS,&cap))return 30;
  configure(0,-1);
 }else configure(kind,phase);
 errno=0;bool accepted=call(input,out);int error=errno;
 if(kind==10&&setrlimit(RLIMIT_AS,&old))return 31;
 int created=0,destroyed=0,last=0,nested=0;counts(&created,&destroyed,&last,&nested);
 bool unchanged=equal(out,before)&&out.domain.data()==storage[0]&&out.name.data()==storage[1]&&out.path.data()==storage[2]&&out.value.data()==storage[3];
 printf("ERROR kind=%d phase=%d accepted=%d errno=%d expected=%d unchanged=%d created=%d destroyed=%d last=%d nested=%d\n",kind,phase,accepted,error,expected,unchanged,created,destroyed,last,nested);
 if(accepted||error!=expected||!unchanged||created!=1||destroyed!=1)return 32;
 if(kind==6&&nested!=1)return 33;
 configure(0,-1);if(!call("recovered=yes",out)||out.name!="recovered"||out.value!="yes"||out.expiry_date_utc!=0)return 34;
 puts("RECOVERY=PASS OUTPUT_UNCHANGED=PASS CLEANUP_SCOPES=1/1");return 0;
}
