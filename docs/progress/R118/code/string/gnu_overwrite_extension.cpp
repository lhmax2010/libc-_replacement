#include "common.h"
int main(){identity();std::string s;s.__resize_and_overwrite(3,[](char*p,std::size_t n){assert(n>=3);p[0]='a';p[1]='b';p[2]='c';return 3;});bytes("overwrite",s);assert(s=="abc");}
