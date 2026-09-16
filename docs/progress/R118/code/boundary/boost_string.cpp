#include "common.h"
#include <dlfcn.h>
#include <boost/test/tree/test_unit.hpp>
#include <boost/program_options/detail/convert.hpp>
int main(int argc,char**argv){
 setvbuf(stdout,nullptr,_IONBF,0);assert(argc==2);
 void* g=dlopen("libstdc++.so.6",RTLD_NOW|RTLD_GLOBAL);void* c=dlopen("libc++.so.1",RTLD_NOW|RTLD_GLOBAL);assert(g&&c);identity();
 FILE* f=fopen("/proc/self/maps","r");assert(f);char line[2048];bool found=false;while(fgets(line,sizeof line,f)){if(strstr(line,argv[1])){printf("PROVIDER_MAP %s",line);found=true;}}fclose(f);assert(found);
 {
#ifdef TEST_BOOST_TEST
  std::string r=boost::unit_test::ut_detail::normalize_test_case_name("& alpha/beta+gamma ");
  bytes("return",r);assert(r=="alpha_beta_gamma");
#else
  std::string input("a\0bc",4);std::string r=boost::program_options::to_internal(input);
  bytes("input",input);bytes("return",r);assert(r==input);
#endif
 }
 printf("consumer_destroy_completed=1\n");
}
