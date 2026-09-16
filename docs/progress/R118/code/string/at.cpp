#include "common.h"
#include <stdexcept>
int main(){identity();std::string s="abc";int caught=0;try{(void)s.at(3);}catch(const std::out_of_range&e){caught=1;std::printf("exception=out_of_range what=%s\n",e.what());}bytes("after",s);assert(caught==1 && s=="abc");std::printf("valid_at_1=%d\n",int(s.at(1)));}
