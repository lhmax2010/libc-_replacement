#include "common.h"
#include <vector>
#include <stdexcept>
int main(){identity();std::vector<int>v={5,9};int caught=0;try{(void)v.at(2);}catch(const std::out_of_range&e){caught=1;printf("exception=out_of_range what=%s\n",e.what());}printf("size=%zu values=%d,%d\n",v.size(),v[0],v[1]);assert(caught&&v[0]==5&&v[1]==9);}
