#include "common.h"
int main() {
 identity(); std::string s; std::size_t cap=s.capacity(); std::printf("push size=0 capacity=%zu\n",cap);
 for(int i=0;i<300;++i) {s.push_back('a'); if(s.capacity()!=cap){cap=s.capacity();std::printf("push size=%zu capacity=%zu\n",s.size(),cap);} assert(s.size()==std::size_t(i+1));}
 for(std::size_t n: {0,1,15,16,22,23,30,31,32,63,64,100,129}) {
  std::string r="abc";r.reserve(n);std::printf("reserve request=%zu size=%zu capacity=%zu\n",n,r.size(),r.capacity());assert(r=="abc" && r.capacity()>=n);
 }
 std::string r(200,'b');r.resize(3);auto old=r.capacity();r.shrink_to_fit();std::printf("shrink before=%zu after=%zu\n",old,r.capacity());assert(r=="bbb");
}
