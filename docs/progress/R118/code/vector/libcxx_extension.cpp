#include "common.h"
#include <vector>
int main(){identity();std::vector<int>v={1,2};bool b=v.__invariants();printf("invariants=%d size=%zu\n",int(b),v.size());assert(b);}
