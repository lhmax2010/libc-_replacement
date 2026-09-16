#include "common.h"
#include <functional>
#include <map>
int main(){identity();std::string a="a",z="z",longer(100,'a');
 int az=a.compare(z),rev=z.compare(a),len=a.compare(longer),same=a.compare("a");
 std::printf("compare_az=%d compare_za=%d compare_length=%d compare_equal=%d\n",az,rev,len,same);assert(az<0&&rev>0&&len<0&&same==0);
 for(const auto& s:{std::string(),a,std::string("abc"),std::string("a\0b",3)})std::printf("hash size=%zu value=%zu\n",s.size(),std::hash<std::string>{}(s));
 std::map<std::string,int> m{{"z",2},{"a",1}};std::printf("map_order=");for(const auto&x:m)std::printf("%s:%d,",x.first.c_str(),x.second);std::printf("\n");assert(m.begin()->first=="a");
}
