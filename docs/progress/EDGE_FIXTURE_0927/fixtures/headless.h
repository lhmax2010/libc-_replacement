#pragma once
#include <dali/devel-api/adaptor-framework/offscreen-application.h>
inline Dali::OffscreenApplication unstarted_application(){
 int n=1;char name[]="edge-fixture";char*args[]={name,nullptr};char**p=args;
 return Dali::OffscreenApplication::New(&n,&p,Dali::OffscreenApplication::FrameworkBackend::GLIB,Dali::OffscreenApplication::RenderMode::MANUAL);
}
