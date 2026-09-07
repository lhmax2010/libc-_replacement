#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>
#include <mutex>
#include <pthread.h>
#include <shared_mutex>
#include <unistd.h>

std::__shared_mutex_base base;
std::shared_mutex plain;
std::shared_timed_mutex timed;
std::atomic<pthread_mutex_t*> native{nullptr};
std::atomic<int> clean{0};
thread_local bool observe = false;
const char* mode;
using Wait = int(*)(pthread_cond_t*,pthread_mutex_t*);
using Timed = int(*)(pthread_cond_t*,pthread_mutex_t*,const timespec*);
using Clock = int(*)(pthread_cond_t*,pthread_mutex_t*,clockid_t,const timespec*);
extern "C" int pthread_cond_wait(pthread_cond_t* c,pthread_mutex_t* m) {
  static auto f=reinterpret_cast<Wait>(dlsym(RTLD_NEXT,"pthread_cond_wait"));
  if(observe) native.store(m,std::memory_order_release);
  return f(c,m);
}
extern "C" int pthread_cond_timedwait(pthread_cond_t* c,pthread_mutex_t* m,const timespec* t) {
  static auto f=reinterpret_cast<Timed>(dlsym(RTLD_NEXT,"pthread_cond_timedwait"));
  if(observe) native.store(m,std::memory_order_release);
  return f(c,m,t);
}
extern "C" int pthread_cond_clockwait(pthread_cond_t* c,pthread_mutex_t* m,clockid_t k,const timespec* t) {
  static auto f=reinterpret_cast<Clock>(dlsym(RTLD_NEXT,"pthread_cond_clockwait"));
  if(observe) native.store(m,std::memory_order_release);
  return f(c,m,k,t);
}
struct Guard {~Guard(){++clean;}};
struct Custom {
  using rep=long long; using period=std::ratio<1>; using duration=std::chrono::duration<rep>;
  using time_point=std::chrono::time_point<Custom>; static constexpr bool is_steady=false;
  static time_point now(){return time_point(duration(0));}
};
void* worker(void*) {
  Guard g; observe=true;
  if(!strcmp(mode,"base")) base.lock();
  else if(!strcmp(mode,"plain")) plain.lock();
  else if(!strcmp(mode,"timed_lock")) timed.lock();
  else if(!strcmp(mode,"system")) timed.try_lock_until(std::chrono::system_clock::now()+std::chrono::hours(1));
  else if(!strcmp(mode,"steady")) timed.try_lock_for(std::chrono::hours(1));
  else timed.try_lock_until(Custom::now()+Custom::duration(3600));
  return reinterpret_cast<void*>(1);
}
int main(int argc,char**argv){
  if(argc!=2)return 64; mode=argv[1]; setvbuf(stdout,nullptr,_IONBF,0);
  alarm(8);
  bool b=!strcmp(mode,"base"),p=!strcmp(mode,"plain");
  if(b)base.lock_shared();else if(p)plain.lock_shared();else timed.lock_shared();
  pthread_t t; if(pthread_create(&t,nullptr,worker,nullptr))return 65;
  while(!native.load(std::memory_order_acquire))usleep(1000);
  auto m=native.load();
  int before=pthread_mutex_lock(m); if(before)return 66;
  unsigned state=b?base.__state_:0;
  pthread_mutex_unlock(m);
  printf("native_wait_entered=1 pre_native_lock=%d state=%u\n",before,state);
  int cancel=pthread_cancel(t);void* result=nullptr;int join=pthread_join(t,&result);
  int unlocked=pthread_mutex_trylock(m); if(!unlocked)pthread_mutex_unlock(m);
  if(b)base.unlock_shared();else if(p)plain.unlock_shared();else timed.unlock_shared();
  bool reader=b?base.try_lock_shared():(p?plain.try_lock_shared():timed.try_lock_shared_for(std::chrono::milliseconds(30)));
  if(reader){if(b)base.unlock_shared();else if(p)plain.unlock_shared();else timed.unlock_shared();}
  bool writer=b?base.try_lock():(p?plain.try_lock():timed.try_lock_for(std::chrono::milliseconds(30)));
  if(writer){if(b)base.unlock();else if(p)plain.unlock();else timed.unlock();}
  bool flag=!b || (base.__state_ & base.__write_entered_);
  printf("cancel=%d join=%d canceled=%d cleanup=%d internal_mutex_rc=%d reader=%d writer=%d explicit_flag=%s\n",cancel,join,result==PTHREAD_CANCELED,clean.load(),unlocked,reader,writer,b?(flag?"1":"0"):"NOT_OBSERVED");
  bool ok=!cancel&&!join&&result==PTHREAD_CANCELED&&clean==1&&!unlocked&&!reader&&!writer&&flag;
  printf("ASSERTIONS=%s\n",ok?"PASS":"FAIL");
  _Exit(ok?0:67); // 损坏对象不调用析构；进程隔离探针。
}
