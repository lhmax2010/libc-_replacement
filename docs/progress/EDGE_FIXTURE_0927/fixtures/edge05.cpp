#include "identity.h"
#include <json/json.h>
int main(int argc,char**argv){identity(argc,argv);Lifetime c;
 {Owned<Json::Reader> reader(c);Json::Value value;std::string text="{\"n\":42,\"name\":\"alpha\",\"v\":[1,2,3]}";
 bool ok=reader->parse(text.data(),text.data()+text.size(),value,false);assert(ok);assert(value["n"].asInt()==42);assert(value["name"].asString()=="alpha");assert(value["v"].size()==3 && value["v"][2].asInt()==3);
 std::cout<<"VALUES parsed="<<ok<<" n="<<value["n"].asInt()<<" name="<<value["name"].asString()<<" last="<<value["v"][2].asInt()<<'\n';
 std::string malformed="{";bool bad=reader->parse(malformed.data(),malformed.data()+malformed.size(),value,false);assert(!bad);auto errors=reader->getFormattedErrorMessages();assert(!errors.empty());std::cout<<"VALUES malformed="<<bad<<" error="<<errors;
 assert(reader->parse(text.data(),text.data()+text.size(),value,false));assert(value["n"].asInt()==42);}
 lifecycle(c,1);}
