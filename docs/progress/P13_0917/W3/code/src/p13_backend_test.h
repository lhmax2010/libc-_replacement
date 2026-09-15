#pragma once
// Test-only synchronous fault injection. Never compiled into package payload.
#include <new>
#include <stdexcept>
namespace p13_test {
thread_local int kind=0,at=-1,created=0,destroyed=0,last=-1,nested=0;
struct Scope { Scope(){++created;} ~Scope(){++destroyed;} };
struct Custom { int value; };
struct ThrowOnExit { bool contain; ~ThrowOnExit() noexcept(false) {
  if(contain){try{throw Custom{271};}catch(...){++nested;}}
  else throw Custom{314};
} };
inline void point(int phase){
 last=phase;if(phase!=at)return;
 switch(kind){
  case 1:throw std::bad_alloc();
  case 2:{std::string x;x.reserve(x.max_size()+1);break;}
  case 3:throw std::invalid_argument("P13 injected invalid argument; not observed parser path");
  case 4:throw Custom{314};
  case 5:throw std::runtime_error("P13 injected standard exception");
  case 6:{ThrowOnExit guard{true};throw std::bad_alloc();}
  case 7:{ThrowOnExit guard{false};throw std::bad_alloc();}
  case 8:{ThrowOnExit guard{false};break;}
  case 9:try{throw Custom{11};}catch(...){throw std::invalid_argument("replacement exception");}
 }
}
}
P12_EXPORT void p13_test_configure(int kind,int phase){p13_test::kind=kind;p13_test::at=phase;p13_test::created=0;p13_test::destroyed=0;p13_test::last=-1;p13_test::nested=0;}
P12_EXPORT void p13_test_counts(int*created,int*destroyed,int*last,int*nested){if(created)*created=p13_test::created;if(destroyed)*destroyed=p13_test::destroyed;if(last)*last=p13_test::last;if(nested)*nested=p13_test::nested;}
#define P13_SCOPE p13_test::Scope p13_scope
#define P13_POINT(i) p13_test::point(i)
