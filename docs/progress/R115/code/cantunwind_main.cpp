#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
extern "C" { int worker_entered=0; void* cantunwind_worker(void*); }
void aborted(int){static const char text[]="OBSERVED_SIGABRT=1\n";write(1,text,sizeof(text)-1);_Exit(134);}
int main(){
 setvbuf(stdout,nullptr,_IONBF,0);alarm(8);signal(SIGABRT,aborted);
 pthread_t t;int create=pthread_create(&t,nullptr,cantunwind_worker,nullptr);
 if(create)return 65;
 while(!__atomic_load_n(&worker_entered,__ATOMIC_ACQUIRE))usleep(1000);
 int cancel=pthread_cancel(t);void*result=nullptr;int join=pthread_join(t,&result);
 printf("create=%d cancel=%d join=%d canceled=%d\n",create,cancel,join,result==PTHREAD_CANCELED);
 return !cancel&&!join&&result==PTHREAD_CANCELED?0:66;
}
