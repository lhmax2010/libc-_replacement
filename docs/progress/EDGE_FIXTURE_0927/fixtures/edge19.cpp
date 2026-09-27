#include "identity.h"
#include <zypp/ResPool.h>
int main(int argc,char**argv){identity(argc,argv);Lifetime c;
 {Owned<zypp::LocaleSet> wanted(c);wanted->insert(zypp::Locale("en"));wanted->insert(zypp::Locale("de"));
 auto pool=zypp::ResPool::instance();auto original=pool.getRequestedLocales();
 pool.setRequestedLocales(wanted.get());auto actual=pool.getRequestedLocales();
 assert(actual==wanted.get());assert(pool.isRequestedLocale(zypp::Locale("en")));assert(pool.isRequestedLocale(zypp::Locale("de")));
 std::cout<<"VALUES count="<<actual.size()<<" en="<<pool.isRequestedLocale(zypp::Locale("en"))<<" de="<<pool.isRequestedLocale(zypp::Locale("de"))<<'\n';
 pool.setRequestedLocales(original);assert(pool.getRequestedLocales()==original);}
 lifecycle(c,1);}
