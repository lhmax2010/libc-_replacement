#include "bridge.h"
#include <cerrno>
#include <cstdio>
#include <limits>
#include <string>
struct Capture { std::string fields[4]; int count=0; bool fail=false; };
static int capture(void* ptr,unsigned i,const char* data,size_t size) {
  auto& c=*static_cast<Capture*>(ptr);++c.count;
  if(c.fail)return ENOMEM;
  if(i>=4 || (!data&&size))return EINVAL;
  c.fields[i].assign(data?data:"",size);return 0;
}
int main() {
  P12CookieState state{{{"keep",4},{"old",3},{"/keep",5},{"value",5}},17};
  P12CookieResult result{999,19};Capture out;int tests=0;
  auto expect=[&](const char*name,int actual,int wanted){printf("UNIT=%s actual=%d expected=%d\n",name,actual,wanted);++tests;return actual==wanted;};
  if(!expect("null_input",p12_cookie_parse({nullptr,1},&state,&out,capture,&result),-EINVAL))return 1;
  if(!expect("null_state",p12_cookie_parse({"",0},nullptr,&out,capture,&result),-EINVAL))return 2;
  if(!expect("null_context",p12_cookie_parse({"",0},&state,nullptr,capture,&result),-EINVAL))return 3;
  if(!expect("null_callback",p12_cookie_parse({"",0},&state,&out,nullptr,&result),-EINVAL))return 4;
  if(!expect("null_result",p12_cookie_parse({"",0},&state,&out,capture,nullptr),-EINVAL))return 5;
  if(!expect("max_length",p12_cookie_parse({"x",std::numeric_limits<size_t>::max()},&state,&out,capture,&result),-EOVERFLOW))return 6;
  auto invalid=state;invalid.fields[2]={nullptr,1};
  if(!expect("null_field",p12_cookie_parse({"x",1},&invalid,&out,capture,&result),-EINVAL))return 7;
  invalid=state;invalid.fields[2]={"x",std::numeric_limits<size_t>::max()};
  if(!expect("oversized_field",p12_cookie_parse({"x",1},&invalid,&out,capture,&result),-EOVERFLOW))return 8;
  invalid=state;for(auto&f:invalid.fields)f={"x",std::string().max_size()};
  if(!expect("aggregate_overflow",p12_cookie_parse({"x",std::string().max_size()},&invalid,&out,capture,&result),-EOVERFLOW))return 9;
  if(!expect("empty_null_allowed",p12_cookie_parse({nullptr,0},&state,&out,capture,&result),0))return 10;
  if(out.count || result.expiry!=999)return 11;
  out.fail=true;
  if(!expect("emit_failure",p12_cookie_parse({"sid=x",5},&state,&out,capture,&result),-ENOMEM))return 12;
  out.fail=false;out.count=0;
  if(!expect("valid_recovery",p12_cookie_parse({"sid=x",5},&state,&out,capture,&result),1))return 13;
  if(out.count!=4||out.fields[0]!="keep"||out.fields[1]!="sid"||out.fields[2]!="/keep"||out.fields[3]!="x"||result.expiry!=0)return 14;
  puts("PROTOCOL_FIELDS domain=keep name=sid path=/keep value=x expiry=0");
  printf("PROTOCOL_UNIT=PASS checks=%d\n",tests);return 0;
}
