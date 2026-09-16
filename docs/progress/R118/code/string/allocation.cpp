#include "common.h"
#include "fail_new.h"
int main(){identity();std::string s(100,'q'),before=s;auto cap=s.capacity();int caught=0;
 fail_new=true;try{s.reserve(cap+1000);}catch(const std::bad_alloc&){fail_new=false;caught=1;}catch(...){fail_new=false;caught=2;}fail_new=false;
 std::printf("exception=%s size=%zu capacity=%zu unchanged=%d\n",caught==1?"bad_alloc":"unexpected",s.size(),s.capacity(),int(s==before && s.capacity()==cap));assert(caught==1 && s==before && s.capacity()==cap);
}
