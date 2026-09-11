#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
int main(int argc,char**argv){
 if(argc!=2)return 20;setvbuf(stdout,NULL,_IONBF,0);
 const char*keys[]={"DISPLAY","WAYLAND_DISPLAY","XDG_RUNTIME_DIR","DBUS_SESSION_BUS_ADDRESS"};
 for(int i=0;i<4;i++)printf("ENV_PRESENT %s=%d\n",keys[i],getenv(keys[i])!=NULL);
 if(!dlopen("libc++.so.1",RTLD_NOW|RTLD_GLOBAL)||!dlopen("libstdc++.so.6",RTLD_NOW|RTLD_GLOBAL))return 21;
 void*h=dlopen(argv[1],RTLD_NOW|RTLD_LOCAL);if(!h){printf("LOAD_ERROR=%s\n",dlerror());return 22;}
 FILE*f=fopen("/proc/self/maps","r");if(!f)return 23;char line[4096];int cxx=0,gnu=0,provider=0;
 while(fgets(line,sizeof line,f)){fputs(line,stdout);cxx|=strstr(line,"/libc++.so.")!=NULL;gnu|=strstr(line,"/libstdc++.so.")!=NULL;provider|=strstr(line,argv[1])!=NULL;}fclose(f);
 printf("MAP_ASSERT cxx=%d gnu=%d provider=%d\n",cxx,gnu,provider);if(!cxx||!gnu||!provider)return 24;
 int(*init)(void)=dlsym(h,"ewk_init");void*(*context)(void)=dlsym(h,"ewk_context_default_get");
 if(!init||!context)return 25;puts("BEFORE_EWK_INIT");int v=init();printf("EWK_INIT=%d\n",v);
 if(v<=0)return 26;puts("BEFORE_EWK_CONTEXT");void*c=context();printf("EWK_CONTEXT=%p\n",c);
 puts("INITIALIZATION_ONLY; SetSyncBinaryReply NOT_CALLED");return c?0:27;
}
