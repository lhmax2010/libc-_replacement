#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dlfcn.h>
static int exceptions;
static void error(const char*s){++exceptions;printf("BINDER_EXCEPTION %s\n",s?s:"(null)");}
static void argerror(const char*s,const char*p){error(s);printf("PARAM=%s\n",p?p:"(null)");}
static void* symbol(void*h,const char*n){void*p=dlsym(h,n);if(!p){printf("MISSING_SYMBOL %s %s\n",n,dlerror());exit(23);}return p;}
int main(int argc,char**argv){
 if(argc!=3 && argc!=4)return 20;setvbuf(stdout,NULL,_IONBF,0);
 if(!dlopen("libc++.so.1",RTLD_NOW|RTLD_GLOBAL)||!dlopen("libstdc++.so.6",RTLD_NOW|RTLD_GLOBAL)){printf("RUNTIME_ERROR=%s\n",dlerror());return 21;}
 // 在 binder 加载失败时也留下被选 core 的实际映射；不把失败 binder 当成已加载。
 if(!dlopen(argv[2],RTLD_NOW|RTLD_GLOBAL)){printf("CORE_LOAD_ERROR=%s\n",dlerror());return 35;}
 FILE*before=fopen("/proc/self/maps","r");if(!before)return 36;char beforeline[4096];int chosen=0;
 puts("PRE_BINDER_MAPS_BEGIN");while(fgets(beforeline,sizeof beforeline,before)){fputs(beforeline,stdout);chosen|=strstr(beforeline,argv[2])!=NULL;}fclose(before);
 printf("PRE_BINDER_CORE_MAP=%d\n",chosen);if(!chosen)return 37;
 void*h=dlopen(argv[1],RTLD_NOW|RTLD_LOCAL);
 if(!h){printf("BINDER_LOAD_ERROR=%s\n",dlerror());return 22;}
 typedef void(*cb)(const char*);typedef void(*acb)(const char*,const char*);
 void(*reg)(cb,cb,cb,cb,cb,cb,cb,cb,cb,cb,cb)=symbol(h,"SWIGRegisterExceptionCallbacks_NDalic");
 void(*areg)(acb,acb,acb)=symbol(h,"SWIGRegisterExceptionArgumentCallbacks_NDalic");
 reg(error,error,error,error,error,error,error,error,error,error,error);areg(argerror,argerror,argerror);
 void*(*makeint)(int)=symbol(h,"CSharp_Dali_new_Property_Value__SWIG_2");
 void*(*makefloat)(float)=symbol(h,"CSharp_Dali_new_Property_Value__SWIG_3");
 void*(*makestring)(char*)=symbol(h,"CSharp_Dali_new_Property_Value__SWIG_12");
 bool(*getint)(void*,int*)=symbol(h,"CSharp_Dali_Property_Value_Get__SWIG_3");
 bool(*getfloat)(void*,float*)=symbol(h,"CSharp_Dali_Property_Value_Get__SWIG_2");
 bool(*getstring)(void*,char**)=symbol(h,"CSharp_Dali_Property_Value_Get__SWIG_12");
 void(*destroy)(void*)=symbol(h,"CSharp_Dali_delete_Property_Value");
 FILE*f=fopen("/proc/self/maps","r");if(!f)return 24;char line[4096];bool cxx=0,gnu=0,binder=0,core=0;
 while(fgets(line,sizeof line,f)){fputs(line,stdout);cxx|=strstr(line,"/libc++.so.")!=NULL;gnu|=strstr(line,"/libstdc++.so.")!=NULL;binder|=strstr(line,argv[1])!=NULL;core|=strstr(line,argv[2])!=NULL;}fclose(f);
 printf("MAP_ASSERT cxx=%d gnu=%d binder=%d core=%d\n",cxx,gnu,binder,core);if(!cxx||!gnu||!binder||!core)return 25;
 if(argc==4){
  void*(*makecontrol)(void)=symbol(h,"CSharp_Dali_View_New");
  puts("CONTROL_NEW_PRECONDITION_CHECK");void*p=makecontrol();
  printf("CONTROL_NEW result=%p exception_callbacks=%d\n",p,exceptions);
  // 仅验证初始化前提；若成功也不记为属性/虚调用矩阵。
  return p&&!exceptions?0:34;
 }
 int values[]={0,-123456789,2147483647};
 for(unsigned i=0;i<3;i++){void*p=makeint(values[i]);if(!p||exceptions)return 26;int v=17;bool ok=getint(p,&v);printf("INTEGER expected=%d actual=%d valid=%d\n",values[i],v,ok);if(!ok||v!=values[i]||exceptions)return 27;destroy(p);if(exceptions)return 28;}
 void*fp=makefloat(1.25f);if(!fp||exceptions)return 29;float fv=0;bool fok=getfloat(fp,&fv);printf("FLOAT expected=1.25 actual=%.9g valid=%d\n",fv,fok);if(!fok||fv!=1.25f||exceptions)return 30;destroy(fp);
 char*texts[]={"", "abc", "long-string-0123456789-abcdefghijklmnopqrstuvwxyz-ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
 for(unsigned i=0;i<3;i++){void*p=makestring(texts[i]);if(!p||exceptions)return 31;char*actual=NULL;bool ok=getstring(p,&actual);printf("STRING expected=%s actual=%s valid=%d\n",texts[i],actual?actual:"(null)",ok);if(!ok||!actual||strcmp(texts[i],actual)||exceptions)return 32;free(actual);destroy(p);if(exceptions)return 33;}
 puts("EXACT_VALUES=7 CONSUMER_DELETE_COMPLETED=7 CALLBACK_EXCEPTIONS=0");return 0;
}
