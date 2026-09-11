#include "ewk_bridge.h"
#include <ewk_cookie_parser.h>
#include <dlfcn.h>
#include <cstdio>
#include <cstring>
static Eina_Bool(*real_parse)(const std::string&,EWKCookieContents&);
extern "C" int ewk_gnu_configure(const char*path){
 void*h=dlopen(path,RTLD_NOW|RTLD_LOCAL);if(!h)return 0;
 real_parse=reinterpret_cast<decltype(real_parse)>(dlsym(h,"ewk_parse_cookie"));
 Dl_info info{};if(!real_parse||!dladdr(reinterpret_cast<void*>(real_parse),&info))return 0;
 printf("BRIDGE_REAL_FUNCTION_FILE=%s\n",info.dli_fname);
 return strcmp(info.dli_fname,path)==0;
}
extern "C" int ewk_gnu_parse(const char*data,size_t n,void*context,ewk_emit emit,int64_t*expiry){
 if(!real_parse)return -1;
 try{
  std::string input(data,n);EWKCookieContents result{};
  if(!real_parse(input,result))return 0;
  const std::string*fields[]={&result.domain,&result.name,&result.path,&result.value};
  for(unsigned i=0;i<4;i++)if(!emit(context,i,fields[i]->data(),fields[i]->size()))return -2;
  *expiry=result.expiry_date_utc;puts("GNU_BRIDGE_EXACT_FIELDS_EMITTED=4");return 1;
 }catch(...){return -3;}
}
