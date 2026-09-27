#include "identity.h"
#include <appcore_cpp/app_core_base.hh>
// Public abstract base requires consumer-supplied main-loop hooks. This fixture
// tests the event collection without starting a platform main loop; a request
// to enter any of these hooks is an assertion failure, never a simulated loop.
struct App final : tizen_cpp::AppCoreBase {
 void OnLoopInit(int,char**)override{assert(false);}
 void OnLoopFinish()override{assert(false);}
 void OnLoopRun()override{assert(false);}
 void OnLoopExit()override{assert(false);}
};
struct Event final : tizen_cpp::AppCoreBase::EventBase {
 int& destruction;int observed=-1;
 explicit Event(int&d):EventBase(Type::LOW_MEMORY),destruction(d){}
 ~Event()override{++destruction;}
 void OnEvent(int n)override{observed=n;}
 void OnEvent(const std::string&)override{assert(false);}
};
int main(int argc,char**argv){identity(argc,argv);Lifetime c;int destroyed=0;
 {Owned<App> app(c);auto event=std::make_shared<Event>(destroyed);std::weak_ptr<Event> weak=event;
 event->SetVal(42);assert(event->GetVal(0)==42);app->AddEvent(event);
 app->RaiseEvent(43,Event::Type::LOW_MEMORY);assert(event->observed==43);
 assert(app->RemoveEvent(event));assert(!app->RemoveEvent(event));event.reset();assert(weak.expired());assert(destroyed==1);
 std::cout<<"VALUES stored=42 callback=43 removed=1 removed_again=0 event_destroyed="<<destroyed<<'\n';}
 lifecycle(c,1);}
