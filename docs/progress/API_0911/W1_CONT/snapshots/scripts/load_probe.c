#include <dlfcn.h>
#include <stdio.h>
int main(int argc,char**argv) {
  if(argc!=2)return 10;
  void*p=dlopen(argv[1],RTLD_NOW|RTLD_LOCAL);
  if(!p){printf("LOAD_ERROR=%s\n",dlerror());return 11;}
  FILE*f=fopen("/proc/self/maps","r");if(!f)return 12;
  char line[4096];while(fgets(line,sizeof line,f))fputs(line,stdout);fclose(f);
  puts("LOAD_ONLY_PASS; no target values measured");return 0;
}
