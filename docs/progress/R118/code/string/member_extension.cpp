#include "common.h"
int main(){identity();std::string s="before";s.__resize_default_init(3);s[0]='n';s[1]='e';s[2]='w';bytes("resized",s);assert(s=="new");}
