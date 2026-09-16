#include "common.h"
#include <array>
int main(){identity();std::array<char,2>a{'b','c'};std::string s="a";s.append_range(a);bytes("appended",s);assert(s=="abc");}
