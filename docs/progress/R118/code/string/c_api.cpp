#include "common.h"
#include <type_traits>
int main(){identity();std::string s("a\0b",3);static_assert(std::is_same<decltype(s.data()),char*>::value,"C++17 mutable data");s.data()[2]='z';
 bytes("embedded_nul",s);std::printf("size=%zu strlen=%zu trailing_nul=%d npos=%zu mutable_data=%d\n",s.size(),std::strlen(s.c_str()),int(s.data()[s.size()]==0),std::string::npos,1);assert(s.size()==3&&s[2]=='z'&&s.data()[3]==0);
 std::string empty;std::printf("empty_nonnull=%d empty_nul=%d\n",empty.data()!=nullptr,int(empty.c_str()[0]==0));
}
