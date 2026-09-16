#include "common.h"
int main(){identity();std::string s="abc";printf("contains_b=%d contains_z=%d\n",s.contains('b'),s.contains('z'));assert(s.contains('b')&&!s.contains('z'));s.resize_and_overwrite(4,[](char*p,std::size_t n){assert(n>=4);p[0]='t';p[1]='e';p[2]='s';p[3]='t';return 4;});bytes("overwrite",s);assert(s=="test");}
