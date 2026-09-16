#include "common.h"
#include <functional>
#include <stdexcept>
int main(){identity();int calls=0;std::function<void()>f=[&](){++calls;throw std::invalid_argument("target-input");};int caught=0;try{f();}catch(const std::invalid_argument&e){caught=1;printf("exception=invalid_argument what=%s\n",e.what());}printf("calls=%d nonempty=%d caught=%d\n",calls,bool(f),caught);assert(calls==1&&f&&caught);}
