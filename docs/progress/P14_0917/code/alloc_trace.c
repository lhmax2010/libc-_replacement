// Linux/glibc test instrument; not a production allocator or recovery mechanism.
#define _GNU_SOURCE
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unwind.h>
#include <dlfcn.h>
extern void* __libc_malloc(size_t);
extern void* __libc_calloc(size_t,size_t);
extern void* __libc_realloc(void*,size_t);
extern void __libc_free(void*);
typedef struct Record {void*p;size_t size,attempt;uintptr_t pc[20];int depth,live,failed;} Record;
static __thread Record records[4096] __attribute__((tls_model("initial-exec")));
static __thread size_t used,attempt,fail_at,failures,overflow;
static __thread int enabled,tracing;
static _Unwind_Reason_Code frame(struct _Unwind_Context*c,void*arg){Record*r=arg;if(r->depth==20)return _URC_END_OF_STACK;r->pc[r->depth++]=_Unwind_GetIP(c);return _URC_NO_REASON;}
static void capture(Record*r){tracing=1;_Unwind_Backtrace(frame,r);tracing=0;}
static Record*prepare(size_t n){
 if(!enabled||tracing)return 0;
 ++attempt;if(used==4096){++overflow;return 0;}
 Record*r=&records[used++];r->size=n;r->attempt=attempt;capture(r);
 if(attempt==fail_at){r->failed=1;++failures;errno=ENOMEM;}return r;
}
static void remove_record(void*p){if(!p||tracing)return;for(size_t i=used;i>0;--i)if(records[i-1].live&&records[i-1].p==p){records[i-1].live=0;return;}}
void*malloc(size_t n){Record*r=prepare(n);if(r&&r->failed)return 0;void*p=__libc_malloc(n);if(r){r->p=p;r->live=p!=0;}return p;}
void*calloc(size_t n,size_t s){Record*r=prepare(n*s);if(r&&r->failed)return 0;void*p=__libc_calloc(n,s);if(r){r->p=p;r->live=p!=0;}return p;}
void*realloc(void*p,size_t n){Record*r=n?prepare(n):0;if(r&&r->failed)return 0;void*q=__libc_realloc(p,n);if(q||!n){remove_record(p);if(r){r->p=q;r->live=q!=0;}}return q;}
void free(void*p){remove_record(p);__libc_free(p);}
void trace_begin(void){enabled=0;Record warm={0};capture(&warm);memset(records,0,sizeof records);used=attempt=fail_at=failures=overflow=0;enabled=1;}
void trace_fail_after(size_t n){fail_at=n?attempt+n:0;}
void trace_stop(void){enabled=0;fail_at=0;}
void trace_result(size_t*live,size_t*bytes,size_t*failed){*live=*bytes=0;*failed=failures;for(size_t i=0;i<used;++i)if(records[i].live){++*live;*bytes+=records[i].size;}}
void trace_dump(void){
 enabled=0;
 for(size_t i=0;i<used;++i){Record*r=&records[i];printf("ALLOC attempt=%zu size=%zu ptr=%p live=%d failed=%d stack=",r->attempt,r->size,r->p,r->live,r->failed);
 for(int j=0;j<r->depth;++j)printf("%s%lx",j?",":"",(unsigned long)r->pc[j]);puts("");
 for(int j=0;j<r->depth;++j){Dl_info d={0};if(dladdr((void*)r->pc[j],&d))printf("FRAME attempt=%zu index=%d object=%s offset=%lx symbol=%s delta=%lx\n",r->attempt,j,d.dli_fname,(unsigned long)(r->pc[j]-(uintptr_t)d.dli_fbase),d.dli_sname?d.dli_sname:"?",d.dli_saddr?(unsigned long)(r->pc[j]-(uintptr_t)d.dli_saddr):0);}
 if(r->live){printf("LIVE_BYTES attempt=%zu hex=",r->attempt);for(size_t j=0;j<r->size&&j<64;++j)printf("%02x",((unsigned char*)r->p)[j]);puts("");}
 }
 printf("TRACE_OVERFLOW=%zu\n",overflow);
}
