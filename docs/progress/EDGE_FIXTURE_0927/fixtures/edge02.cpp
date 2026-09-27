#include "identity.h"
#include <bundle_cpp.h>
#include <vector>
int main(int argc,char**argv){identity(argc,argv);Lifetime c;
 {Owned<tizen_base::Bundle> b(c);std::vector<std::string> v={"alpha","中文",std::string(257,'x')};
 int rc=b->Add("values",v);assert(rc==0);auto got=b->GetStringArray("values");assert(got==v);assert(b->GetCount()==1);
 std::cout<<"VALUES rc="<<rc<<" count="<<b->GetCount()<<" n="<<got.size()<<" s0="<<got[0]<<" s1="<<got[1]<<" s2="<<got[2]<<'\n';
 assert(b->Delete("values")==0);assert(b->IsEmpty());}
 lifecycle(c,1);}
