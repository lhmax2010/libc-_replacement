#!/usr/bin/env python3
"""实际源码的封闭投影表达式，两库分别编译；失败逐式隔离，不填参数。"""
from common import *
from expressions import request
import re
CC=pathlib.Path('/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++')
SDK=ROOT/'tmp/API_0911/W1_CONT';BUILD=ROOT/'tmp/IMPL_0908/build-native'
GNU=SDK/'gnu14_sdk/usr/lib64/gcc/x86_64-tizen-linux-gnu/14.2.0/include/c++'
TMP=ROOT/'tmp/R119_PROJECTION';TMP.mkdir(parents=True,exist_ok=True)
PREAMBLE=r'''#include <cstdio>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <iterator>
#include <memory>
#include <functional>
#include <string>
#include <vector>
#include <tuple>
#include <array>
#include <utility>
#include <chrono>
#include <ratio>
#include <complex>
struct EmptyControl {};
template<class T> const char* actual() { return __PRETTY_FUNCTION__; }
template<class T> struct base {using type=typename std::remove_cv<T>::type;};
template<class T> struct base<T*> :base<T>{};
template<class T> struct base<T&> :base<T>{};
template<class T> struct base<T&&> :base<T>{};
template<class T, std::size_t N> struct base<T[N]> :base<T>{};
template<class T> void emit(const char* id) {
 using B=typename base<T>::type;
 std::printf("%s\tTYPE=%s\tBASE=%s\t",id,actual<T>(),actual<B>());
 if constexpr(std::is_void<B>::value) std::printf("kind=void\tsize=NA\tempty=NA\n");
 else if constexpr(std::is_function<B>::value) std::printf("kind=function\tsize=NA\tempty=NA\n");
 else std::printf("kind=%s\tsize=%zu\tempty=%d\tclass=%d\tunion=%d\tenum=%d\n",
 std::is_arithmetic<B>::value||std::is_null_pointer<B>::value?"builtin":"other",sizeof(B),int(std::is_empty<B>::value),int(std::is_class<B>::value),int(std::is_union<B>::value),int(std::is_enum<B>::value));
}
int main(){
 // 正向/负向测法对照，不来自候选，不计入真实投影结果。
 static_assert(std::is_empty<EmptyControl>::value,"control empty");
 static_assert(!std::is_empty<std::string>::value,"control object");
 emit<void>("CONTROL_VOID");emit<int>("CONTROL_BUILTIN");emit<EmptyControl>("CONTROL_EMPTY");emit<std::string>("CONTROL_OBJECT");
'''
def flags(lib):
 common=['-std=c++17','-O0','-g','-Wno-deprecated-declarations','-nostdinc++','-nostdlib++','-fuse-ld=/usr/bin/ld.gold']
 if lib=='gnu':return common+['-I'+str(GNU),'-I'+str(GNU/'x86_64-tizen-linux-gnu')],[SDK/'closure_ewk/libstdc++.so.6']
 return common+['-I'+str(BUILD/'include/c++/v1')],['-L'+str(BUILD/'lib'),'-lc++','-lc++abi']
def test_group(lib,items):
 if not items:return
 signature=hashlib.sha256(('\n'.join(k for k,e in items)).encode()).hexdigest()[:16]
 label=f'{lib}_{signature}';src=OUT/'probes'/f'{label}.cpp';src.parent.mkdir(parents=True,exist_ok=True)
 src.write_text(PREAMBLE+''.join(f' emit<{expr}>("{key}");\n' for key,expr in items)+'}\n')
 exe=TMP/label;opts,links=flags(lib);rc,_,raw=record('build_'+label,[CC,*opts,src,*links,'-pthread','-o',exe])
 if rc:
  if len(items)>1:
   mid=len(items)//2;test_group(lib,items[:mid]);test_group(lib,items[mid:]);return
  key,expr=items[0];save(OUT/'measurements'/f'{key}_{lib}.json',dict(expression=expr,library=lib,status='COMPILE_UNRESOLVED',build_exit=rc,build_record=raw,reason='真实封闭表达式在本次 C++17 标准头环境不成立；保留编译错误，不改模板实参。'));return
 rc,_,layout=record('layout_'+label,[CC,*opts,'-Xclang','-fdump-record-layouts','-c',src,'-o',TMP/(label+'.o')])
 if rc:raise RuntimeError('layout command failed '+label)
 runs=[]
 for i in range(1,6):
  rc,txt,runref=record(f'run_{label}_{i}',[SDK/'closure_ewk/ld-linux-x86-64.so.2','--library-path',str(BUILD/'lib')+':'+str(SDK/'closure_ewk'),exe])
  if rc:raise RuntimeError('nonzero runtime '+runref)
  values={s.split('\t')[0]:s for s in txt.splitlines() if '\t' in s}
  assert 'kind=void' in values['CONTROL_VOID'] and 'kind=builtin' in values['CONTROL_BUILTIN']
  assert 'empty=1' in values['CONTROL_EMPTY'] and 'empty=0' in values['CONTROL_OBJECT']
  assert set(k for k,e in items)<=set(values)
  runs.append((values,runref))
 for key,expr in items:
  assert len({r[0][key] for r in runs})==1
  save(OUT/'measurements'/f'{key}_{lib}.json',dict(expression=expr,library=lib,status='MEASURED',source=str(src.relative_to(ROOT)),source_sha256=sha(src),executable_sha256=sha(exe),build_record=raw,layout_record=layout,runs=[r[1] for r in runs],result=runs[0][0][key],repeat_count=5,architecture='x86_64'))
def main(end):
 gate(f'measure_gate_{end}');start=time.time();queue={}
 for c in candidates()[:end]:
  with gzip.open(OUT/f'occurrences/{int(c["rank"]):03}.json.gz','rt') as f:rows=json.load(f)
  for h in rows:
   expr,why=request(h)
   if expr:
    key=hashlib.sha256(expr.encode()).hexdigest()[:24];queue[key]=expr
 save(OUT/f'CONCRETE_REQUESTS_{end:02}.json',queue)
 for lib in ['gnu','cxx']:
  todo=[(k,e) for k,e in queue.items() if not (OUT/'measurements'/f'{k}_{lib}.json').exists()]
  for i in range(0,len(todo),80):
   gate(f'measure_gate_{end}_{lib}_{i}');test_group(lib,todo[i:i+80])
 save(OUT/f'MEASURE_TIME_{end:02}.json',dict(elapsed_seconds=time.time()-start,requested_candidates=end,distinct_concrete_expressions=len(queue),method='同一真实表达式可复用一次测量，映射保留所有源命中；每侧五次；失败记录不丢弃'))
 print('MEASURED QUEUE',end,len(queue))
if __name__=='__main__':main(int(sys.argv[1]))
