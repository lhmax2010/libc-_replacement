#include "identity.h"
#include <bundle_cpp.h>
int main(int argc,char**argv){identity(argc,argv);Lifetime c;
 {std::initializer_list<std::pair<std::string,std::string>> kv={{"first","alpha"},{"second","中文"}};
 Owned<tizen_base::Bundle> b(c,kv);assert(b->GetCount()==2);assert(b->GetString("first")=="alpha");assert(b->GetString("second")=="中文");
 std::cout<<"VALUES count="<<b->GetCount()<<" first="<<b->GetString("first")<<" second="<<b->GetString("second")<<'\n';}
 lifecycle(c,1);}
