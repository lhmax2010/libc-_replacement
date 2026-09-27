#include "identity.h"
#include "headless.h"
#include <dali/devel-api/common/singleton-service.h>
#include <dali/public-api/object/handle.h>
#include <dali/public-api/object/weak-handle.h>
struct RegisteredTag{};struct AbsentTag{};
int main(int argc,char**argv){identity(argc,argv);Lifetime life;Dali::WeakHandleBase weak;
 {Owned<Dali::OffscreenApplication> app(life,unstarted_application());identity(argc,argv);
 auto service=Dali::SingletonService::Get();assert(service);
 auto object=Dali::Handle::New();auto index=object.RegisterProperty("fixture-value",42);weak=Dali::WeakHandleBase(object);
 service.Register(typeid(RegisteredTag),object);object.Reset();assert(weak.GetBaseHandle());
 auto found=service.GetSingleton(typeid(RegisteredTag));assert(found==weak.GetBaseHandle());
 auto handle=Dali::Handle::DownCast(found);assert(handle && handle.GetProperty<int>(index)==42);
 auto absent=service.GetSingleton(typeid(AbsentTag));assert(!absent);
 std::cout<<"VALUES registered=1 value="<<handle.GetProperty<int>(index)<<" absent="<<bool(absent)<<" retained_after_input_reset="<<bool(weak.GetBaseHandle())<<'\n';
 handle.Reset();found.Reset();service.Reset();}
 assert(!weak.GetBaseHandle());std::cout<<"VALUES released_after_application="<<!bool(weak.GetBaseHandle())<<'\n';lifecycle(life,1);}
