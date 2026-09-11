#include <dali/devel-api/adaptor-framework/offscreen-application.h>
#include <cstdio>
#include <cstring>
#include <dlfcn.h>
int main(int argc,char**argv){
 setvbuf(stdout,nullptr,_IONBF,0);
 if(argc!=2)return 20;
 if(!dlopen("libc++.so.1",RTLD_NOW|RTLD_GLOBAL)||!dlopen("libstdc++.so.6",RTLD_NOW|RTLD_GLOBAL))return 21;
 void*h=dlopen(argv[1],RTLD_NOW|RTLD_LOCAL);if(!h){puts(dlerror());return 22;}
 auto make=reinterpret_cast<void*(*)()>(dlsym(h,"CSharp_Dali_View_New"));
 auto drop=reinterpret_cast<void(*)(void*)>(dlsym(h,"CSharp_Dali_delete_View"));
 if(!make||!drop)return 23;
 FILE*f=fopen("/proc/self/maps","r");if(!f)return 24;char line[4096];int a=0,b=0,c=0,d=0;
 while(fgets(line,sizeof line,f)){fputs(line,stdout);a|=strstr(line,"libc++.so")!=nullptr;b|=strstr(line,"libstdc++.so")!=nullptr;c|=strstr(line,argv[1])!=nullptr;d|=strstr(line,"libdali2-adaptor.so")!=nullptr;}fclose(f);
 printf("MAP_ASSERT cxx=%d gnu=%d binder=%d adaptor=%d\n",a,b,c,d);if(!a||!b||!c||!d)return 25;
 int ac=1;char* av[]={argv[0],nullptr};char**avp=av;
 puts("OFFSCREEN_NEW_BEGIN");
 auto app=Dali::OffscreenApplication::New(&ac,&avp,Dali::OffscreenApplication::FrameworkBackend::ECORE,Dali::OffscreenApplication::RenderMode::MANUAL);
 puts("OFFSCREEN_START_BEGIN");app.Start();
 bool valid=static_cast<bool>(app.GetWindow());printf("OFFSCREEN_WINDOW_VALID=%d\n",valid);
 void*view=make();printf("BINDER_VIEW_NONNULL=%d\n",view!=nullptr);if(view)drop(view);
 puts("BINDER_DELETE_RETURNED");app.Terminate();puts("OFFSCREEN_TERMINATE_RETURNED");
 return valid&&view?0:26;
}
