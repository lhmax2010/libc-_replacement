#include "identity.h"
#include <zypp/ZConfig.h>
int main(int argc,char**argv){identity(argc,argv);Lifetime c;
 {Owned<std::set<std::string>> wanted(c);wanted->insert("kernel-default");wanted->insert("provides:multiversion(kernel)");
 auto&cfg=zypp::ZConfig::instance();auto original=cfg.multiversionSpec();cfg.multiversionSpec(wanted.get());
 assert(cfg.multiversionSpec()==wanted.get());std::cout<<"VALUES count="<<cfg.multiversionSpec().size();for(const auto&s:cfg.multiversionSpec())std::cout<<" item="<<s;std::cout<<'\n';
 cfg.multiversionSpec(original);assert(cfg.multiversionSpec()==original);}
 lifecycle(c,1);}
