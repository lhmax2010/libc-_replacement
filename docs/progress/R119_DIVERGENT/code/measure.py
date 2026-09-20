#!/usr/bin/env python3
"""固定清单，两库实际展开与类型编码，五轮；缺失类型保留失败，不换实参。"""
from common import *
import re
CC=pathlib.Path('/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++')
SDK=ROOT/'tmp/API_0911/W1_CONT';BUILD=ROOT/'tmp/IMPL_0908/build-native'
GNU=SDK/'gnu14_sdk/usr/lib64/gcc/x86_64-tizen-linux-gnu/14.2.0/include/c++'
TMP=ROOT/'tmp/R119_DIVERGENT';TMP.mkdir(parents=True,exist_ok=True)
PRE=r'''#include <cstdio>
#include <cstddef>
#include <cstdint>
#include <cwchar>
#include <cmath>
#include <type_traits>
#include <typeinfo>
#include <limits>
#include <ios>
#include <string>
#include <string_view>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <array>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <queue>
#include <iterator>
#include <chrono>
#include <random>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <filesystem>
#include <functional>
#include <memory>
#include <atomic>
#include <regex>
#include <exception>
#include <typeindex>
#include <initializer_list>
template<class T> struct TypeToken {};
template<class T> __attribute__((used,noinline)) void abi_identity(TypeToken<T>) {}
template<class T> const char* actual(){return __PRETTY_FUNCTION__;}
template<class T> void emit(const char* id){
  abi_identity(TypeToken<T>{});
  std::printf("%s\tTYPE=%s\tTOKEN=%s\tDIRECT=%s\t",id,actual<T>(),typeid(TypeToken<T>).name(),typeid(T).name());
  if constexpr(std::is_void<T>::value||std::is_function<T>::value) std::printf("SIZE=NA\tALIGN=NA\n");
  else std::printf("SIZE=%zu\tALIGN=%zu\n",sizeof(T),alignof(T));
}
'''
def flags(lib):
 opts=['-std=c++17','-O0','-g','-Wno-deprecated-declarations','-nostdinc++','-nostdlib++','-fuse-ld=/usr/bin/ld.gold']
 if lib=='gnu':return opts+['-I'+str(GNU),'-I'+str(GNU/'x86_64-tizen-linux-gnu')],[SDK/'closure_ewk/libstdc++.so.6']
 return opts+['-I'+str(BUILD/'include/c++/v1')],['-L'+str(BUILD/'lib'),'-lc++','-lc++abi']
def source(items):
 definitions=[];calls=[]
 for r in items:
  expr=r['expression'];id=r['id']
  # 对具体容器/类的成员，用依赖探测记录“此实例无成员”，不替换请求类型。
  m=re.match(r'^(std::.*)::([a-z][a-z0-9_]*)$',expr)
  if m and ('<' in m[1] or m[1].count('::')>=2 or m[1] in ['std::string','std::wstring','std::u16string','std::u32string','std::string_view','std::wstring_view','std::ios_base','std::thread','std::mutex','std::recursive_mutex','std::timed_mutex','std::recursive_timed_mutex','std::shared_mutex','std::condition_variable','std::regex','std::wregex','std::cmatch','std::smatch','std::csub_match','std::ssub_match','std::random_device']):
   parent,member=m.groups()
   definitions.append(f'template<class C,class=void> struct Probe_{id}{{static void run(){{std::printf("{id}\\tUNAVAILABLE=member {member}\\n");}}}};\ntemplate<class C> struct Probe_{id}<C,std::void_t<typename C::{member}>>{{static void run(){{emit<typename C::{member}>("{id}");}}}};\n')
   calls.append(f'Probe_{id}<{parent}>::run();')
  else:calls.append(f'emit<{expr}>("{id}");')
 return PRE+'\n'.join(definitions)+'\nint main(){emit<int>("CONTROL_INT");emit<long>("CONTROL_LONG");emit<long long>("CONTROL_LONG_LONG");emit<const long&>("CONTROL_CONST_REF");\n'+'\n'.join(calls)+'\n}\n'
def run_group(lib,items):
 gate(f'measure_gate_{lib}_{items[0]["id"]}_{len(items)}')
 label='v2_'+lib+'_'+hashlib.sha256(','.join(r['id'] for r in items).encode()).hexdigest()[:16]
 src=OUT/'probes'/f'{label}.cpp';src.parent.mkdir(parents=True,exist_ok=True);src.write_text(source(items));exe=TMP/label
 opts,links=flags(lib);rc,out,build=record('build_'+label,[CC,*opts,src,*links,'-pthread','-o',exe])
 if rc:
  if len(items)>1:
   mid=len(items)//2;run_group(lib,items[:mid]);run_group(lib,items[mid:]);return
  save(OUT/'measurements'/f'{items[0]["id"]}_{lib}.json',dict(probe_version=2,status='NOT_AVAILABLE',reason='请求表达式在本次 C++17 环境编译失败，未替换实参',expression=items[0]['expression'],build_record=build,build_exit=rc));return
 rc,syms,symref=record('symbols_'+label,['readelf','-Ws','--wide',exe]);assert rc==0 and 'abi_identity' in syms
 runs=[]
 for i in range(1,6):
  rc,out,ref=record(f'run_{label}_{i}',[SDK/'closure_ewk/ld-linux-x86-64.so.2','--library-path',str(BUILD/'lib')+':'+str(SDK/'closure_ewk'),exe]);assert rc==0
  values={s.split('\t')[0]:s for s in out.splitlines() if '\t' in s}
  for name,code in [('INT','i'),('LONG','l'),('LONG_LONG','x'),('CONST_REF','RKl')]:assert 'TOKEN=9TypeTokenI'+code+'E\t' in values['CONTROL_'+name]
  assert set(r['id'] for r in items)<=set(values)
  runs.append((values,ref))
 for r in items:
  assert len({values[r['id']] for values,ref in runs})==1
  line=runs[0][0][r['id']];result=dict(probe_version=2,expression=r['expression'],library=lib,architecture='x86_64',source=str(src.relative_to(ROOT)),source_sha256=sha(src),executable_sha256=sha(exe),build_record=build,symbol_record=symref,runs=[ref for values,ref in runs],repeat_count=5,output=line)
  if '\tUNAVAILABLE=' in line:result.update(status='NOT_AVAILABLE',reason=line.split('UNAVAILABLE=')[1]+'；请求成员类型在本次访问上下文不可形成（可能未声明或不可访问），不推断不存在；探测五轮一致')
  else:
   pretty=re.search(r'TYPE=.*?\[T = (.*?)\]',line)[1];token=re.search(r'TOKEN=([^\t]+)',line)[1];assert token.startswith('9TypeTokenI') and token.endswith('E')
   code=re.search(r'DIRECT=([^\t]+)',line)[1]
   result.update(status='MEASURED',actual_type=pretty,type_encoding=code,wrapper_encoding=token,encoding_note='DIRECT 是 typeid(T) 的独立编码，会去除顶层 cv/ref；完整 TOKEN 保留 cv/ref，用于判相同。不剥除包装后单独解码，以免破坏替换表。',size=re.search(r'SIZE=([^\t]+)',line)[1],alignment=re.search(r'ALIGN=(.*)$',line)[1])
  save(OUT/'measurements'/f'{r["id"]}_{lib}.json',result)
def main():
 rows=json.loads((OUT/'CATALOG.json').read_text());start=time.time()
 for lib in ['gnu','cxx']:
  todo=[]
  for r in rows:
   p=OUT/'measurements'/f'{r["id"]}_{lib}.json'
   if p.exists():
    m=json.loads(p.read_text())
    if m.get('probe_version')==2:continue
    save(OUT/'measurements_pass1'/p.name,m)
   todo.append(r)
  for i in range(0,len(todo),48):run_group(lib,todo[i:i+48])
 summary=[]
 for r in rows:
  a,b=[json.loads((OUT/'measurements'/f'{r["id"]}_{lib}.json').read_text()) for lib in ['gnu','cxx']]
  if a['status']!= 'MEASURED' or b['status']!='MEASURED':status='NOT_AVAILABLE'
  else:status='IDENTICAL' if a['wrapper_encoding']==b['wrapper_encoding'] else 'DIVERGENT'
  summary.append(dict(**r,result=status,gnu_type=a.get('actual_type',a.get('reason')),cxx_type=b.get('actual_type',b.get('reason')),gnu_encoding=a.get('type_encoding','NOT_OBSERVED'),cxx_encoding=b.get('type_encoding','NOT_OBSERVED'),gnu_wrapper=a.get('wrapper_encoding','NOT_OBSERVED'),cxx_wrapper=b.get('wrapper_encoding','NOT_OBSERVED'),gnu_size=a.get('size','NOT_OBSERVED'),cxx_size=b.get('size','NOT_OBSERVED'),gnu_evidence=f'measurements/{r["id"]}_gnu.json',cxx_evidence=f'measurements/{r["id"]}_cxx.json'))
 tsv(OUT/'TYPE_RESULTS.tsv',summary);save(OUT/'TYPE_RESULTS.json',summary);tsv(OUT/'DIVERGENT_TYPES.tsv',[r for r in summary if r['result']=='DIVERGENT'])
 save(OUT/'MEASUREMENT_SUMMARY.json',dict(cells=len(summary),distribution=dict(__import__('collections').Counter(r['result'] for r in summary)),elapsed_seconds=time.time()-start,architecture='x86_64',standard='c++17',repeats=5,identity='same exact TypeToken<T> type encoding; pretty spelling retained; not a layout equivalence claim'))
 print((OUT/'MEASUREMENT_SUMMARY.json').read_text())
if __name__=='__main__':main()
