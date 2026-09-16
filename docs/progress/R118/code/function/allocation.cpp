#include "common.h"
#include "fail_new.h"
#include <functional>
struct Big{char payload[256]{};int operator()()const{return 91;}};
int main(){identity();std::function<int()>f=[](){return 23;};Big b;int caught=0;fail_new=true;try{f=b;}catch(const std::bad_alloc&){fail_new=false;caught=1;}catch(...){fail_new=false;caught=2;}fail_new=false;printf("exception=%s old_value=%d\n",caught==1?"bad_alloc":"unexpected",f());assert(caught==1&&f()==23);}
