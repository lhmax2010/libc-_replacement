#include <atomic>
#include <codecvt>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <locale>
#include <ostream>
#include <pthread.h>
#include <streambuf>
#include <unistd.h>

std::atomic<int> ready{0},go{0},returned{0},cleanup{0},blocked_flush{0};
pthread_mutex_t native_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t native_cv=PTHREAD_COND_INITIALIZER;
void check(bool ok,const char*label){if(!ok){printf("FAIL=%s\n",label);_Exit(70);}}
struct Sink:std::streambuf{
  int_type overflow(int_type ch) override{
    if(go){
      blocked_flush=1;
      check(pthread_mutex_lock(&native_mutex)==0,"native_lock");
      pthread_cond_wait(&native_cv,&native_mutex);
      pthread_mutex_unlock(&native_mutex);
    }
    return ch;
  }
};
struct Cleanup{~Cleanup(){++cleanup;puts("BUFFER cleanup=1");}};
void terminal(){printf("event=terminate returned=%d blocked_flush=%d cleanup=%d\n",returned.load(),blocked_flush.load(),cleanup.load());_Exit(86);}
void* worker(void*){
  check(pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,nullptr)==0,"disable");
  Cleanup clean;Sink sink;
  {
    std::wbuffer_convert<std::codecvt_utf8<wchar_t>> converted(&sink);
    {std::wostream stream(&converted);stream<<L"abcdef";check(!stream.fail(),"buffer_write");}
    ready=1;while(!go)usleep(1000);
    check(pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,nullptr)==0,"enable_deferred");
  }
  returned=1;
  pthread_testcancel();return nullptr;
}
int main(){
  std::set_terminate(terminal);
  pthread_t t;check(pthread_create(&t,nullptr,worker,nullptr)==0,"create");
  while(!ready)usleep(1000);check(pthread_cancel(t)==0,"cancel_request");go=1;
  void* result=nullptr;check(pthread_join(t,&result)==0,"join");
  check(result==PTHREAD_CANCELED && returned==1 && blocked_flush==0 && cleanup==1,"deferred_cancel_after_destructor");
  puts("BUFFER_CANCEL canceled=1 destructor_returned=1 blocked_flush=0 cleanup=1 ASSERTIONS=PASS");
}
