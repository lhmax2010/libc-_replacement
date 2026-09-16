#include "common.h"
#include "fail_new.h"
int main() {
  identity(); std::printf("sizeof=%zu alignof=%zu\n",sizeof(std::string),alignof(std::string));
  for(std::size_t n=0;n<=40;++n) {
    long before=new_calls; std::string s(n,'x');
    auto b=reinterpret_cast<std::uintptr_t>(&s), p=reinterpret_cast<std::uintptr_t>(s.data());
    bool in=p>=b && p<b+sizeof(s);
    std::printf("n=%zu capacity=%zu allocations=%ld inline=%d data_offset=%ld\n",n,s.capacity(),new_calls-before,int(in),in?long(p-b):-1L);
    assert(s.size()==n && s.c_str()[n]=='\0');
  }
}
