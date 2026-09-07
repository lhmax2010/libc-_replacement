#include <unistd.h>
// 仅设定进程总时限，不改变任何工作线程的调用帧或异常表。
__attribute__((constructor)) static void probe_deadline() { alarm(8); }
