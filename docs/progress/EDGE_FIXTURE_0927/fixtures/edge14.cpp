#include "identity.h"
#include <dali/devel-api/threading/conditional-wait.h>
#include <chrono>
int main(int argc,char**argv){identity(argc,argv);Lifetime c;
 {Owned<Dali::ConditionalWait> wait(c);auto start=std::chrono::steady_clock::now();auto end=start+std::chrono::milliseconds(20);
 {Dali::ConditionalWait::ScopedLock lock(wait.get());while(std::chrono::steady_clock::now()<end)wait->WaitUntil(lock,end);assert(&lock.GetLockedWait()==&wait.get());}
 {Dali::ConditionalWait::ScopedLock again(wait.get());assert(&again.GetLockedWait()==&wait.get());}
 auto elapsed=std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now()-start).count();assert(elapsed>=20000);assert(wait->GetWaitCount()==0);
 std::cout<<"VALUES elapsed_us="<<elapsed<<" wait_count="<<wait->GetWaitCount()<<" reacquire=1\n";}
 lifecycle(c,1);}
