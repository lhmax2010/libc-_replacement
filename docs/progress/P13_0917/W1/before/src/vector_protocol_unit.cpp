#include "bridge.h"
#include <cerrno>
#include <cstdio>
#include <limits>
int main(){
 int a=p12_vector_bytes(0,nullptr,1);
 int b=p12_vector_bytes(0,reinterpret_cast<const uint8_t*>(1),std::numeric_limits<size_t>::max());
 int c=p12_vector_bytes(0,nullptr,0);
 printf("VECTOR_PROTOCOL null=%d oversized=%d empty=%d\n",a,b,c);
 return a==-EINVAL&&b==-EOVERFLOW&&c==1?0:1;
}
