// 仅探针进程安装信号记录器，避免板端 crash-manager 遗留诊断包；不改变系统设置。
// 观测故障信号后直接失败退出，绝不把它计作字段/销毁通过。
#include <signal.h>
#include <unistd.h>
static void api_fault(int sig){
 if(sig==SIGSEGV){const char s[]="OBSERVED_SIGNAL=SIGSEGV; probe exits 139 without core dump\n";write(2,s,sizeof(s)-1);}
 else if(sig==SIGABRT){const char s[]="OBSERVED_SIGNAL=SIGABRT; probe exits 134 without core dump\n";write(2,s,sizeof(s)-1);}
 else if(sig==SIGBUS){const char s[]="OBSERVED_SIGNAL=SIGBUS\n";write(2,s,sizeof(s)-1);}
 else if(sig==SIGILL){const char s[]="OBSERVED_SIGNAL=SIGILL\n";write(2,s,sizeof(s)-1);}
 else if(sig==SIGFPE){const char s[]="OBSERVED_SIGNAL=SIGFPE\n";write(2,s,sizeof(s)-1);}
 else {const char s[]="PROBE_DEADLINE_EXCEEDED\n";write(2,s,sizeof(s)-1);}
 _exit(sig==SIGALRM?124:128+sig);
}
__attribute__((constructor)) static void api_monitor_init(void){signal(SIGSEGV,api_fault);signal(SIGABRT,api_fault);signal(SIGBUS,api_fault);signal(SIGILL,api_fault);signal(SIGFPE,api_fault);signal(SIGALRM,api_fault);alarm(30);}
