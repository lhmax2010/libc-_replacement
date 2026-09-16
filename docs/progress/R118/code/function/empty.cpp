#include "common.h"
#include <functional>
int main(){identity();std::function<void()>f;int caught=0;try{f();}catch(const std::bad_function_call&e){caught=1;printf("exception=bad_function_call what=%s\n",e.what());}printf("empty=%d caught=%d\n",!f,caught);assert(caught&&!f);}
