#include <ewk_cookie_parser.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>
static EWKCookieContents initial(){EWKCookieContents o{};o.domain=std::string(128,'d');o.name="old";o.path=std::string(128,'p');o.value="value";o.expiry_date_utc=999;return o;}
static bool same(const EWKCookieContents&a,const EWKCookieContents&b){return a.domain==b.domain&&a.name==b.name&&a.path==b.path&&a.value==b.value&&a.expiry_date_utc==b.expiry_date_utc;}
int main(int argc,char**argv){
 if(argc!=3)return 20;setvbuf(stdout,nullptr,_IONBF,0);
 void*h=dlopen(argv[1],RTLD_NOW|RTLD_LOCAL);if(!h){puts(dlerror());return 21;}
 auto call=reinterpret_cast<Eina_Bool(*)(const std::string&,EWKCookieContents&)>(dlsym(h,"ewk_parse_cookie"));
 auto fail=reinterpret_cast<void(*)(int)>(dlsym(h,"p12_test_fail_field"));if(!call||!fail)return 22;
 int expected=atoi(argv[2]);
 if(expected){auto out=initial();auto before=out;errno=0;bool ok=call("sid=abc",out);int e=errno;printf("LOADER_ERROR ok=%d errno=%d unchanged=%d\n",ok,e,same(out,before));return !ok&&e==expected&&same(out,before)?0:28;}
 for(int i=0;i<4;++i){auto out=initial();auto before=out;fail(i);errno=0;bool ok=call("sid=abc; Domain=new; Path=/new",out);int e=errno;printf("FAULT=%d ok=%d errno=%d unchanged=%d\n",i,ok,e,same(out,before));if(ok||e!=ENOMEM||!same(out,before))return 28;fail(-1);}
 {EWKCookieContents out;out.domain="keep";out.path="/keep";if(!call("sid=abc",out)||out.name!="sid"||out.value!="abc"||out.domain!="keep"||out.path!="/keep"||out.expiry_date_utc!=0)return 29;puts("UNINITIALIZED_OLD_EXPIRY success=1 new_expiry=0");}
 {auto out=initial();auto d=out.domain.data();auto p=out.path.data();if(!call("sid=abc",out)||out.domain.data()!=d||out.path.data()!=p)return 30;puts("ABSENT_FIELD_STORAGE preserved=1");}
 FILE*f=fopen("/proc/self/maps","r");if(!f)return 31;char line[4096];while(fgets(line,sizeof line,f))fputs(line,stdout);fclose(f);
 puts("FAULT_SUITE=PASS");return 0;
}
