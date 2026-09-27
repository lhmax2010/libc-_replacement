#include "identity.h"
#include <zypp/parser/xml/Reader.h>
#include <zypp/base/InputStream.h>
#include <zypp/base/Exception.h>
#include <typeinfo>
int main(int argc,char**argv){identity(argc,argv);Lifetime c;
 {Owned<std::istringstream> stream(c,"<root><item>42</item></root>");Owned<zypp::InputStream> input(c,stream.get(),"memory-fixture");
 Owned<zypp::xml::Reader> reader(c,input.get(),zypp::xml::Validate::none());
 std::vector<std::string> names,values;
 do{names.push_back((*reader.get()).name().asString());values.push_back((*reader.get()).value().asString());}while(reader->nextNode());
 assert(names.size()==5);assert(names[0]=="root"&&names[1]=="item"&&names[3]=="item"&&names[4]=="root");assert(values[2]=="42");
 std::cout<<"VALUES nodes="<<names.size();for(size_t i=0;i<names.size();++i)std::cout<<" ["<<names[i]<<","<<values[i]<<"]";std::cout<<'\n';}
 lifecycle(c,3);
 // Reader.h's public example catches zypp::Exception for parse errors.
 Lifetime badLife;bool typed=false;
 {Owned<std::istringstream> stream(badLife,"<root>");Owned<zypp::InputStream> input(badLife,stream.get(),"malformed-memory");
 try{zypp::xml::Reader reader(input.get());while(reader.nextNode()){};}
 catch(const zypp::Exception&error){typed=true;std::cout<<"VALUES parse_error_base=zypp::Exception dynamic_type="<<typeid(error).name()<<" message="<<error.asString()<<'\n';}}
 assert(typed);lifecycle(badLife,2);}
