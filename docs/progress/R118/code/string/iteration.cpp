#include "common.h"
int main(){identity();std::string s="abcd";s.reserve(100);auto addr=reinterpret_cast<std::uintptr_t>(s.data());auto cap=s.capacity();
 s.push_back('e');std::printf("within_capacity_address_same=%d capacity=%zu value=%s\n",int(addr==reinterpret_cast<std::uintptr_t>(s.data())),s.capacity(),s.c_str());
 s.reserve(cap+100);std::printf("beyond_capacity_address_same=%d capacity=%zu value=%s\n",int(addr==reinterpret_cast<std::uintptr_t>(s.data())),s.capacity(),s.c_str());assert(s=="abcde");
 std::printf("iteration=");for(char c:s)std::printf("%c",c);std::printf("\n");
 // 仅比较保存的整数地址，不使用旧迭代器；地址相同不证明迭代器仍有效。
}
