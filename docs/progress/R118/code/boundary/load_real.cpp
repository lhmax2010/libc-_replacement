#include "common.h"
#include <dlfcn.h>
int main(int argc,char**argv){assert(argc==2||argc==3);setvbuf(stdout,nullptr,_IONBF,0);assert(dlopen("libstdc++.so.6",RTLD_NOW|RTLD_GLOBAL));assert(dlopen("libc++.so.1",RTLD_NOW|RTLD_GLOBAL));identity();if(argc==3){void*w=dlopen(argv[2],RTLD_NOW|RTLD_GLOBAL);if(!w){printf("FRONT_NOT_LOADED=%s\n",dlerror());return 21;}printf("FRONT_LOADED=%s\n",argv[2]);}void*h=dlopen(argv[1],RTLD_NOW|RTLD_LOCAL);if(!h){printf("PROVIDER_NOT_LOADED=%s\n",dlerror());return 20;}printf("PROVIDER_LOADED=%s\n",argv[1]);dlclose(h);}
