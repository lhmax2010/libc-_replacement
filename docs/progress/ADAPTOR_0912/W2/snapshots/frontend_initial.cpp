#include "bridge.h"
#include <ewk_cookie_parser.h>
#include <stdexcept>
static int emit(void*p,unsigned i,const char*s,size_t n){
 auto&out=*static_cast<EWKCookieContents*>(p);
 std::string*f[]={&out.domain,&out.name,&out.path,&out.value};
 if(i>=4)return 0;try{f[i]->assign(s,n);return 1;}catch(...){return 0;}
}
extern "C" __attribute__((visibility("default"))) int api_probe_configure_real_provider(const char*p){return bridge_configure(p);}
extern "C" __attribute__((visibility("default"))) Eina_Bool ewk_parse_cookie(const std::string&in,EWKCookieContents&out){
 CookieState initial{{{out.domain.data(),out.domain.size()},{out.name.data(),out.name.size()},
                       {out.path.data(),out.path.size()},{out.value.data(),out.value.size()}},out.expiry_date_utc};
 EWKCookieContents next{};int64_t expiry=0;
 int rc=bridge_parse({in.data(),in.size()},&initial,&next,emit,&expiry);
 // Experimental policy: fail visibly; do not silently turn transport/allocation failure into success.
 // This is NOT a claim of exception-contract equivalence with the original function.
 if(rc<0)throw std::runtime_error("experimental adaptor transport/allocation error");
 if(rc==0)return EINA_FALSE;
 out.domain.swap(next.domain);out.name.swap(next.name);out.path.swap(next.path);out.value.swap(next.value);
 out.expiry_date_utc=static_cast<time_t>(expiry);return EINA_TRUE;
}
