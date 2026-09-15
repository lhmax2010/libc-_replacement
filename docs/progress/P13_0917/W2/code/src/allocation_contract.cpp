#include "bridge.h"
#include <string>
#include <cstdio>
#include <cerrno>
#include <sys/resource.h>
static int emit(void*,unsigned,const char*,size_t){return 0;}
int main(){
 setvbuf(stdout,nullptr,_IONBF,0);
 P12CookieState initial{{{"keep",4},{"old",3},{"/keep",5},{"value",5}},0};
 P12CookieResult result{};int context=0;
 // Load the real provider before reducing address space; no allocation of
 // the claimed huge input occurs in the test itself.
 if(p12_cookie_parse({"sid=x",5},&initial,&context,emit,&result)!=1)return 20;
 rlimit limit{1024ULL*1024*1024,1024ULL*1024*1024};if(setrlimit(RLIMIT_AS,&limit))return 21;
 for(auto&field:initial.fields)field={"x",std::string().max_size()};
 puts("EXPECTED: backend returns -ENOMEM; caller remains alive");
 int rc=p12_cookie_parse({"x",1},&initial,&context,emit,&result);
 printf("ACTUAL_RETURN=%d EXPECTED=%d\n",rc,-ENOMEM);
 return rc==-ENOMEM?0:28;
}
