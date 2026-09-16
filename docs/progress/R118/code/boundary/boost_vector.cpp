#include "common.h"
#include <vector>
#include <dlfcn.h>
#include <boost/program_options/parsers.hpp>
int main(int argc,char**argv){
 setvbuf(stdout,nullptr,_IONBF,0);assert(argc==2);assert(dlopen("libstdc++.so.6",RTLD_NOW|RTLD_GLOBAL));assert(dlopen("libc++.so.1",RTLD_NOW|RTLD_GLOBAL));identity();
 FILE*f=fopen("/proc/self/maps","r");assert(f);char line[2048];bool found=false;while(fgets(line,sizeof line,f))if(strstr(line,argv[1])){found=true;printf("PROVIDER_MAP %s",line);}fclose(f);assert(found);
 {
 auto v=boost::program_options::split_unix("alpha 'two words' omega");printf("size=%zu\n",v.size());assert(v.size()==3);for(std::size_t i=0;i<v.size();++i)bytes("element",v[i]);assert(v[0]=="alpha"&&v[1]=="two words"&&v[2]=="omega");
 }
 printf("consumer_destroy_completed=1\n");
}
