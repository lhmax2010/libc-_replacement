#include "common.h"
#include <utility>
int main() {
 identity(); std::string empty, cstr("a\0bc"), counted("a\0bc",4);
 const char arr[]={'r','a','n','g','e'}; std::string range(arr,arr+5);
 std::string copy=counted, moved=std::move(copy), assigned; assigned=range;
 std::string moveassigned; moveassigned=std::move(assigned);
 bytes("empty",empty);bytes("cstr",cstr);bytes("counted",counted);bytes("range",range);
 bytes("copy_then_moved",moved);bytes("moved_from",copy);bytes("moveassigned",moveassigned);
 assert(empty.empty() && cstr=="a" && moved==counted && moveassigned==range);
 std::string longsrc(200,'z'); std::string longmove=std::move(longsrc);
 std::printf("long_move_size=%zu source_size=%zu\n",longmove.size(),longsrc.size());assert(longmove==std::string(200,'z'));
}
