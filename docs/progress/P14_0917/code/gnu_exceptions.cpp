#include <string>
#include <new>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>
#define P12_EXPORT
#include "p13_backend_test.h"
int main(int argc,char**argv) {
  setvbuf(stdout,nullptr,_IONBF,0);
  if(argc!=2)return 20;
  int kind=atoi(argv[1]);
  if(kind==0 && !dlopen("libc++.so.1",RTLD_NOW|RTLD_LOCAL))return 21;
  FILE*f=fopen("/proc/self/maps","r");if(!f)return 22;
  char line[4096];bool g=false,c=false;
  while(fgets(line,sizeof line,f)){fputs(line,stdout);g|=strstr(line,"/libstdc++.so.")!=nullptr;c|=strstr(line,"/libc++.so.")!=nullptr;}
  fclose(f);printf("MAP_ASSERT gnu=%d cxx=%d positive=%d\n",g,c,kind==0);
  if(!g || c!=(kind==0))return 23;
  if(kind==0)return 0;
  p13_test_configure(kind,0);
  int caught=0;
  try {p13_test::Scope guard;p13_test::point(0);}
  catch(const std::bad_alloc&){caught=1;}
  catch(const std::invalid_argument&){caught=2;}
  catch(const p13_test::Custom&e){if(e.value!=314)return 24;caught=3;}
  catch(...){caught=4;}
  printf("GNU_ONLY kind=%d caught=%d created=%d destroyed=%d nested=%d\n",kind,caught,p13_test::created,p13_test::destroyed,p13_test::nested);
  if(p13_test::created!=1||p13_test::destroyed!=1)return 25;
  if(kind==6)return caught==1&&p13_test::nested==1?0:26;
  if(kind==8)return caught==3?0:27;
  if(kind==9)return caught==2?0:28;
  return 29;
}
