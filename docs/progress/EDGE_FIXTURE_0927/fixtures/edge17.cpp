#include "identity.h"
#include <gtest/gtest.h>
#include <sstream>
int main(int argc,char**argv){identity(argc,argv);Lifetime c;
 {Owned<std::string> input(c,"a\nb");Owned<std::string> printed(c,::testing::PrintToString(input.get()));assert(printed.get()=="\"a\\nb\"");
 std::cout<<"VALUES escaped="<<printed.get()<<" input_size="<<input->size()<<'\n';assert(input.get()=="a\nb");}
 lifecycle(c,2);}
