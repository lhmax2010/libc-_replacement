#include <ios>
#include <cstdio>
std::streamoff accept_offset(std::streamoff);
int main(){auto actual=accept_offset(13);std::printf("input=13 actual=%lld expected=20\n",(long long)actual);return actual==20?0:1;}
