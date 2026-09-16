#include "common.h"
#include <array>
int main(){identity();std::array<char,3>a{'a','b','c'};std::string s(std::from_range,a);bytes("range",s);assert(s=="abc");}
