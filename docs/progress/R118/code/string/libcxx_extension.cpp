#include "common.h"
int main(){identity();std::string s="invariant";bool v=s.__invariants();std::printf("invariants=%d size=%zu\n",int(v),s.size());assert(v);}
