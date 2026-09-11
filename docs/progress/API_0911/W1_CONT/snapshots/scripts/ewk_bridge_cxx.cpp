#include "ewk_bridge.h"
#include <ewk_cookie_parser.h>
static int receive(void*opaque,unsigned i,const char*p,size_t n){
 auto&result=*static_cast<EWKCookieContents*>(opaque);
 std::string*fields[]={&result.domain,&result.name,&result.path,&result.value};
 if(i>=4)return 0;try{fields[i]->assign(p,n);return 1;}catch(...){return 0;}
}
extern "C" __attribute__((visibility("default"))) int api_probe_configure_real_provider(const char*p){return ewk_gnu_configure(p);}
extern "C" __attribute__((visibility("default"))) Eina_Bool ewk_parse_cookie(const std::string&input,EWKCookieContents&output){
 int64_t expiry=0;int rc=ewk_gnu_parse(input.data(),input.size(),&output,receive,&expiry);
 if(rc==1){output.expiry_date_utc=expiry;return EINA_TRUE;}return EINA_FALSE;
}
