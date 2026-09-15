// Test instrumentation only: count the current thread's malloc-family calls
// during a bounded window. No dlsym, allocation, or C++ inside these wrappers.
// Fixed Linux/glibc fixture; NOT part of the production adaptor or its route.
#include "p13_alloc_report.h"
#include <stdint.h>
#include <string.h>
#include <errno.h>
extern void* __libc_malloc(size_t);
extern void* __libc_calloc(size_t,size_t);
extern void* __libc_realloc(void*,size_t);
extern void __libc_free(void*);
typedef struct Entry { void* pointer;size_t size; } Entry;
static __thread Entry entries[4096] __attribute__((tls_model("initial-exec")));
static __thread P13AllocReport report __attribute__((tls_model("initial-exec")));
static __thread int enabled __attribute__((tls_model("initial-exec")));
static __thread size_t fail_nth,attempts,failed;
static int inject_failure(void){if(enabled&&++attempts==fail_nth){++failed;errno=ENOMEM;return 1;}return 0;}
static void add(void*p,size_t n){
 if(!enabled||!p)return;
 for(size_t i=0;i<4096;++i)if(!entries[i].pointer){entries[i]=(Entry){p,n};++report.allocations;++report.live;report.bytes+=n;return;}
 ++report.overflow;
}
static void remove_entry(void*p){
 if(!p||!report.live)return;
 for(size_t i=0;i<4096;++i)if(entries[i].pointer==p){++report.frees;--report.live;report.bytes-=entries[i].size;entries[i]=(Entry){0,0};return;}
}
void*malloc(size_t n){if(inject_failure())return 0;void*p=__libc_malloc(n);add(p,n);return p;}
void*calloc(size_t n,size_t size){if(inject_failure())return 0;void*p=__libc_calloc(n,size);if(p)add(p,n*size);return p;}
void*realloc(void*p,size_t n){if(n&&inject_failure())return 0;void*q=__libc_realloc(p,n);if(q||n==0){remove_entry(p);add(q,n);}return q;}
void free(void*p){remove_entry(p);__libc_free(p);}
void p13_alloc_begin(void){enabled=0;attempts=0;failed=0;fail_nth=0;memset(entries,0,sizeof entries);memset(&report,0,sizeof report);enabled=1;}
void p13_alloc_end(P13AllocReport*out){enabled=0;fail_nth=0;if(out)*out=report;}
void p13_alloc_fail_on(size_t nth){fail_nth=nth;}
size_t p13_alloc_failed(void){return failed;}
void p13_alloc_fail_after(size_t offset){fail_nth=attempts+offset;}
