#include "common.h"
#include "fail_new.h"
#include <vector>
int main(){identity();std::vector<int>v={2,4,6};auto before=v;auto cap=v.capacity();int caught=0;fail_new=true;try{v.reserve(1000);}catch(const std::bad_alloc&){fail_new=false;caught=1;}catch(...){fail_new=false;caught=2;}fail_new=false;printf("exception=%s unchanged=%d capacity=%zu size=%zu\n",caught==1?"bad_alloc":"unexpected",int(v==before&&v.capacity()==cap),v.capacity(),v.size());assert(caught==1&&v==before&&v.capacity()==cap);}
