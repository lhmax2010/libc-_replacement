#include "identity.h"
#define Uses_SCIM_UTILITY
#include <scim.h>
#include <iomanip>
int main(int argc,char**argv){identity(argc,argv);Lifetime c;
 {Owned<scim::WideString> input(c);input->push_back(0x41);input->push_back(0x4e2d);input->push_back(0x1f642);
 auto result=scim::utf8_wcstombs(input.get());std::string expected="A\xE4\xB8\xAD\xF0\x9F\x99\x82";assert(result==expected);assert(input->size()==3);
 std::cout<<"VALUES size="<<result.size()<<" hex=";for(unsigned char ch:result)std::cout<<std::hex<<std::setw(2)<<std::setfill('0')<<unsigned(ch);std::cout<<std::dec<<'\n';}
 lifecycle(c,1);}
