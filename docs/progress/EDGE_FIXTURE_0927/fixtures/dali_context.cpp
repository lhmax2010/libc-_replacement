#include "identity.h"
#include <dali/devel-api/adaptor-framework/offscreen-application.h>
#include <dali/devel-api/common/singleton-service.h>
// Bounded prerequisite probe, NOT an edge baseline. Uses the public offscreen
// factory, manual rendering, no fake graphics controller, and no event loop.
int main(int argc,char**argv){identity(argc,argv);
 int n=1;char name[]="edge-context";char*args[]={name,nullptr};char**p=args;
 std::cout<<"CONTEXT before_offscreen_new GLIB MANUAL\n"<<std::flush;
 auto app=Dali::OffscreenApplication::New(&n,&p,Dali::OffscreenApplication::FrameworkBackend::GLIB,Dali::OffscreenApplication::RenderMode::MANUAL);
 assert(app);std::cout<<"CONTEXT constructed\n"<<std::flush;
 auto service=Dali::SingletonService::Get();std::cout<<"CONTEXT before_start singleton="<<bool(service)<<'\n'<<std::flush;
 if(service){auto handle=service.GetSingleton(typeid(Lifetime));assert(!handle);std::cout<<"CONTEXT unknown_singleton="<<bool(handle)<<'\n';}
 // Start() was separately observed to require the missing Tizen EGL context.
 // Do not retry it here: determine whether lookup can be tested before Start.
 std::cout<<"CONTEXT leaving_unstarted_application\n";}
