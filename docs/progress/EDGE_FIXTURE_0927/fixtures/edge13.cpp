#include "identity.h"
#include <dali/devel-api/common/hash.h>
#include <string_view>
int main(int argc,char**argv){identity(argc,argv);Lifetime c;
 {Owned<std::string> s(c,"prefix-abcdefghijklmnopqrstuvwxyz-suffix");std::string_view view(s->data()+7,26);std::string copy(view);
 auto a=Dali::CalculateHash(view);auto b=Dali::CalculateHash(copy);assert(a==b);assert(copy=="abcdefghijklmnopqrstuvwxyz");
 auto empty=Dali::CalculateHash(std::string_view());assert(empty==Dali::CalculateHash(std::string()));
 std::cout<<"VALUES view_hash="<<a<<" owned_string_hash="<<b<<" length="<<view.size()<<" empty_hash="<<empty<<'\n';}
 lifecycle(c,1);}
