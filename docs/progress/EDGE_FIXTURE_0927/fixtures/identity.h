#pragma once
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <utility>
// Runner hashes the exact provider file before launch and again after exit.
// The process must find that same canonical path in its startup maps.
inline void identity(int argc,char** argv) {
  assert(argc==3);
  std::ifstream maps("/proc/self/maps"); assert(maps);
  std::set<std::string> paths;std::string line;
  while(std::getline(maps,line)) {auto n=line.find('/');if(n!=std::string::npos) paths.insert(line.substr(n));}
  assert(paths.count(argv[1])==1);
  for(const auto& path:paths)std::cout<<"MAP "<<path<<'\n';
  std::cout<<"PROVIDER_START path="<<argv[1]<<" sha256="<<argv[2]<<'\n';
#ifdef _GLIBCXX_RELEASE
  std::cout<<"STDLIB GNU release="<<_GLIBCXX_RELEASE<<" date="<<__GLIBCXX__<<" cxx11_abi="<<_GLIBCXX_USE_CXX11_ABI<<'\n';
#endif
#ifdef _LIBCPP_VERSION
  std::cout<<"STDLIB libc++ version="<<_LIBCPP_VERSION<<'\n';
#endif
  std::cout.flush();
  assert(sizeof(void*)==8); // This task is x86_64 only.
}
// Counts completion of actual public T construction/destruction by this owner.
// This is not a provider-internal allocator/leak check or a concurrency proof.
struct Lifetime {int constructed=0,destroyed=0;};
template<class T> struct Owned {
  Lifetime& count;std::unique_ptr<T> object;
  template<class... A> Owned(Lifetime& c,A&&...a):count(c),object(new T(std::forward<A>(a)...)){++count.constructed;}
  ~Owned(){object.reset();++count.destroyed;}
  T& get(){return *object;} T* operator->(){return object.get();}
};
inline void lifecycle(const Lifetime& c,int expected) {
  std::cout<<"LIFETIME constructed="<<c.constructed<<" destroyed="<<c.destroyed<<" expected="<<expected<<'\n';
  assert(c.constructed==expected && c.destroyed==expected);
}
