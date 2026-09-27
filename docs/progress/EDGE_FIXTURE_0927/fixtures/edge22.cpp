#include "identity.h"
#include <dali/devel-api/threading/conditional-wait.h>
#include <chrono>
#include <thread>
int main(int argc,char**argv){identity(argc,argv);Lifetime c;
 {Owned<Dali::ConditionalWait> wait(c);int value=0;auto deadline=std::chrono::steady_clock::now()+std::chrono::seconds(2);
 std::thread worker;
 {Dali::ConditionalWait::ScopedLock lock(wait.get());worker=std::thread([&]{Dali::ConditionalWait::ScopedLock other(wait.get());value=42;wait->Notify(other);});
 while(value!=42 && std::chrono::steady_clock::now()<deadline)wait->WaitUntil(lock,deadline);assert(value==42);assert(&lock.GetLockedWait()==&wait.get());}
 worker.join();assert(wait->GetWaitCount()==0);std::cout<<"VALUES notified_value="<<value<<" wait_count="<<wait->GetWaitCount()<<" joined=1\n";}
 lifecycle(c,1);}
