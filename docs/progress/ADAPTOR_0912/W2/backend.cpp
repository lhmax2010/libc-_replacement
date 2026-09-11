#include "bridge.h"
#include <ewk_cookie_parser.h>
#include <cstdio>
#include <cstring>
#include <dlfcn.h>
static Eina_Bool(*real_parse)(const std::string&,EWKCookieContents&);
extern "C" int bridge_configure(const char*path){
 void*h=dlopen(path,RTLD_NOW|RTLD_LOCAL);if(!h){puts(dlerror());return 0;}
 real_parse=reinterpret_cast<decltype(real_parse)>(dlsym(h,"ewk_parse_cookie"));
 Dl_info info{};if(!real_parse||!dladdr(reinterpret_cast<void*>(real_parse),&info))return 0;
 printf("REAL_PROVIDER=%s\n",info.dli_fname);return strcmp(path,info.dli_fname)==0;
}
extern "C" int bridge_parse(ByteSpan input,const CookieState*initial,void*ctx,EmitState emit,int64_t*expiry){
 if(!real_parse||!initial||!emit||!expiry)return -1;
 try{
  std::string text(input.p,input.n);EWKCookieContents out{};
  std::string*fields[]={&out.domain,&out.name,&out.path,&out.value};
#ifndef ADAPTOR_DROP_INITIAL
  for(unsigned i=0;i<4;i++)fields[i]->assign(initial->fields[i].p,initial->fields[i].n);
  out.expiry_date_utc=static_cast<time_t>(initial->expiry);
#endif
  if(!real_parse(text,out))return 0;
  for(unsigned i=0;i<4;i++)if(!emit(ctx,i,fields[i]->data(),fields[i]->size()))return -2;
  *expiry=out.expiry_date_utc;return 1;
 }catch(...){return -3;}
}
