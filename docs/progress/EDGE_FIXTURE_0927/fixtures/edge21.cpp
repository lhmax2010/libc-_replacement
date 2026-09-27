#include "identity.h"
#include <zypp/CheckSum.h>
#include <sstream>
int main(int argc,char**argv){identity(argc,argv);Lifetime c;
 {Owned<std::istringstream> input(c,"abc");Owned<zypp::CheckSum> result(c,std::string("sha256"),input.get());
 assert(result->type()=="sha256");assert(result->checksum()=="ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");assert(!input->bad());
 std::cout<<"VALUES type="<<result->type()<<" checksum="<<result->checksum()<<" bad="<<input->bad()<<'\n';}
 lifecycle(c,2);}
