#include <atomic>
#include <chrono>
#include <codecvt>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>
#include <locale>
#include <mutex>
#include <ostream>
#include <pthread.h>
#include <sstream>
#include <unistd.h>

#ifndef EXPECT_NEW
#error EXPECT_NEW required
#endif
using namespace std::chrono_literals;
std::condition_variable cv;
std::mutex mutex;
std::atomic<int> arrivals{0},done{0};
bool ready=false;
int value=0;
void check(bool p,const char* label){if(!p){printf("FAIL=%s\n",label);_Exit(70);}}
extern "C" int pthread_cond_wait(pthread_cond_t* c,pthread_mutex_t* m){
  static auto f=reinterpret_cast<int(*)(pthread_cond_t*,pthread_mutex_t*)>(dlsym(RTLD_NEXT,"pthread_cond_wait"));
  if(c==cv.native_handle())++arrivals;
  return f(c,m);
}
void* waiter(void*){
  std::unique_lock<std::mutex> lk(mutex);
  cv.wait(lk,[]{return ready;});
  check(lk.owns_lock() && value==42,"predicate_value_and_ownership");
  ++done;return nullptr;
}
void await(int n){
  auto end=std::chrono::steady_clock::now()+4s;
  while(arrivals<n && std::chrono::steady_clock::now()<end)usleep(1000);
  check(arrivals>=n,"native_wait_arrival");
  std::lock_guard<std::mutex> lk(mutex); // wait released the same native lock
}
void cv_normal(){
  pthread_t t;check(pthread_create(&t,nullptr,waiter,nullptr)==0,"create");await(1);
  cv.notify_all();await(2);
  {std::lock_guard<std::mutex> lk(mutex);check(!ready && value==0 && done==0,"spurious_no_progress");ready=true;value=42;}
  cv.notify_one();check(pthread_join(t,nullptr)==0 && done==1,"notify_join");
  std::unique_lock<std::mutex> lk(mutex);
  check(cv.wait_for(lk,20ms)==std::cv_status::timeout && lk.owns_lock(),"steady_timeout_owner");
  check(cv.wait_until(lk,std::chrono::system_clock::now()+20ms)==std::cv_status::timeout && lk.owns_lock(),"system_timeout_owner");
  check(!cv.wait_for(lk,20ms,[]{return false;}),"predicate_timeout");
  puts("CV_NORMAL value=42 completed=1 spurious_no_progress=1 steady_timeout=1 system_timeout=1 lock_owned=1");
}
void buffer_normal(bool explicit_sync){
  std::stringbuf sink;int sync_rc=-999;std::string before;
  {std::wbuffer_convert<std::codecvt_utf8<wchar_t>> converted(&sink);
   {std::wostream stream(&converted);stream<<L"abcdef";check(!stream.fail(),"write_succeeded");}
   before=sink.str();check(before=="a","pending_buffer_positive_control");
   if(explicit_sync)sync_rc=converted.pubsync();
  }
  const std::string after=sink.str();
  const std::string expected=(explicit_sync || !EXPECT_NEW)?"abcdef":"a";
  check(after==expected,"exact_output");if(explicit_sync)check(sync_rc==0,"pubsync_success");
  printf("BUFFER_NORMAL explicit=%d before=%s after=%s bytes=%zu lost=%zu sync_rc=%d\n",explicit_sync,before.c_str(),after.c_str(),after.size(),6-after.size(),sync_rc);
}
int main(int argc,char**argv){
  if(argc!=2)return 64;
  if(!strcmp(argv[1],"cv"))cv_normal();
  else if(!strcmp(argv[1],"pubsync"))buffer_normal(true);
  else if(!strcmp(argv[1],"destructor"))buffer_normal(false);
  else return 64;
  puts("ASSERTIONS=PASS");return 0;
}
