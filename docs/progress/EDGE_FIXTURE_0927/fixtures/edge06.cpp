#include "identity.h"
#include <absl/base/call_once.h>
#include <atomic>
#include <thread>
#include <chrono>
int main(int argc,char**argv){identity(argc,argv);Lifetime c;
 {Owned<absl::once_flag> flag(c);std::atomic<bool> entered{false},release{false};std::atomic<int> count{0};int value=0;
 std::thread first([&]{absl::call_once(flag.get(),[&]{++count;entered=true;while(!release.load())std::this_thread::yield();value=42;});});
 while(!entered.load())std::this_thread::yield();
 std::thread second([&]{absl::call_once(flag.get(),[&]{++count;value=-1;});assert(value==42);});
 std::this_thread::sleep_for(std::chrono::milliseconds(150));release=true;first.join();second.join();
 assert(count==1 && value==42);std::cout<<"VALUES callable_count="<<count<<" value="<<value<<'\n';}
 lifecycle(c,1);}
