#!/usr/bin/env python3
"""两侧同一份逐项结构探针；记录实际类型展开/值/空性，不做包间运行实验。"""
from common import *
import importlib.util, re
spec=importlib.util.spec_from_file_location('r118run',ROOT/'docs/progress/R118/code/run.py')
cfg=importlib.util.module_from_spec(spec);spec.loader.exec_module(cfg)
plan=json.loads((OUT/'code/plan.json').read_text()); data={x['rank']:x for x in inputs()}
start=time.time();gate('a_gate')
code=['''#include <cstdio>
#include <cstddef>
#include <ios>
#include <limits>
#include <type_traits>
#include <iterator>
#include <tuple>
#include <memory>
#include <chrono>
#include <functional>
#include <utility>
#include <variant>
#include <string>
struct Base {}; struct Derived:Base {}; enum class Enum:unsigned {zero}; struct Fn { int operator()() const; };
template<class T> const char* actual_type() {return __PRETTY_FUNCTION__;}
template<class T> void type_result(int rank, const char* name, const char* kind) {
  if constexpr (std::is_void<T>::value) std::printf("%d\\t%s\\t%s\\t%s\\tsize=NA\\tempty=NA\\n",rank,name,kind,actual_type<T>());
  else std::printf("%d\\t%s\\t%s\\t%s\\tsize=%zu\\tempty=%d\\n",rank,name,kind,actual_type<T>(),sizeof(T),int(std::is_empty<T>::value));
}
int main() {
  static_assert(!std::is_empty<int>::value, "非空对照");
  static_assert(std::is_empty<Base>::value, "空类对照");
  std::printf("CONTROL empty_Base=%d nonempty_int=%d unequal_builtin=%d\\n",int(std::is_empty<Base>::value),int(!std::is_empty<int>::value),int(!std::is_same<int,long>::value));
  std::printf("VERSION libcxx=%d gcc=%d\\n",
#ifdef _LIBCPP_VERSION
  _LIBCPP_VERSION,
#else
  0,
#endif
#ifdef _GLIBCXX_RELEASE
  _GLIBCXX_RELEASE
#else
  0
#endif
  );
''']
cases=[]
for group in ['A_BUILTIN_ALIAS','A_EMPTY_TYPE','A_COMPILE_TIME_ONLY']:
    for rank_s,expr in plan[group].items():
        rank=int(rank_s);name=data[rank]['row']['candidate'];kind=group[2:]
        if expr.endswith('::value') or expr.endswith('::max()'):
            code.append(f'  constexpr auto value_{rank} = {expr};\n  std::printf("{rank}\\t{name}\\t{kind}\\tvalue=%lld\\n",(long long)value_{rank});\n')
        else:
            code.append(f'  type_result<{expr}>({rank},"{name}","{kind}");\n')
            if group=='A_EMPTY_TYPE': code.append(f'  static_assert(std::is_empty<{expr}>::value, "rank {rank} 实测空类条件");\n')
        cases.append(dict(rank=rank,candidate=name,subtype=kind,expression=expr,old_sample=data[rank]['dossier']['review']['note']))
code.append('}\n')
source=OUT/'code/structure_probe.cpp';source.write_text(''.join(code)); save(OUT/'A_CASES.json',cases)
tmp=ROOT/'tmp/R119_RECLASSIFY';tmp.mkdir(parents=True,exist_ok=True); results=[]
for lib in ['gnu','cxx']:
    flags,links,build,gnu_lib=cfg.configuration('x86_64',lib);exe=tmp/('structure_'+lib)
    flags+=['-Wno-deprecated-declarations']
    begin=time.time();rc,_=record('a_'+lib+'_build',[cfg.CC,*flags,source,*links,'-pthread','-o',exe])
    if rc:raise SystemExit(rc)
    rc,pre=record('a_'+lib+'_preprocess',[cfg.CC,*flags,'-E',source])
    if rc:raise SystemExit(rc)
    rc,_=record('a_'+lib+'_layout',[cfg.CC,*flags,'-Xclang','-fdump-record-layouts','-c',source,'-o',tmp/('structure_'+lib+'.o')])
    if rc:raise SystemExit(rc)
    runs=[]
    for repeat in range(1,6):
        rc,text=record(f'a_{lib}_run{repeat}',[cfg.SDK/'closure_ewk/ld-linux-x86-64.so.2','--library-path',str(build/'lib')+':'+str(cfg.SDK/'closure_ewk'),exe])
        if rc:raise SystemExit(rc)
        assert 'CONTROL empty_Base=1 nonempty_int=1 unequal_builtin=1' in text
        parsed={int(x.split('\t')[0]):x for x in text.splitlines() if '\t' in x}
        assert len(parsed)==len(cases)
        runs.append(parsed)
    assert all(r==runs[0] for r in runs)
    # 从本次实际预处理的头文件位置取源码证据，不靠名称猜一份未参与构建的头。
    located=[];path='';line=0
    for text in pre.splitlines():
        m=re.match(r'# (\d+) "([^"]+)"',text)
        if m:path=m[2];line=int(m[1]);continue
        located.append((path,line,text));line+=1
    definitions={}
    for case in cases:
        token=case['candidate'].split('::')[-1]; matches=[]; seen=set()
        for path,line,text in located:
            if path==str(source) or not re.search(r'\b'+re.escape(token)+r'\b',text):continue
            p=pathlib.Path(path)
            if not p.is_file() or (path,line) in seen:continue
            seen.add((path,line));lines=p.read_text(errors='replace').splitlines()
            # 所有命中均保存；包括声明与定义，不能以 synopsis 代替实现。
            matches.append(dict(path=path,line=line,preprocessed=text,sha256=sha(p),source='\n'.join(f'{i+1}: {lines[i]}' for i in range(max(0,line-4),min(len(lines),line+20)))))
        definitions[str(case['rank'])]=matches
    save(OUT/f'A_DEFINITIONS_{lib}.json',definitions)
    results.append(dict(library=lib,compiler=str(cfg.CC),source_sha256=sha(source),executable_sha256=sha(exe),build_seconds=time.time()-begin,results=runs[0],repeat_count=5,definitions_without_token=[c['candidate'] for c in cases if not definitions[str(c['rank'])]]))
save(OUT/'A_RESULTS.json',results)
save(OUT/'A_EXECUTION_TIME.json',dict(start_epoch=start,end_epoch=time.time(),elapsed_seconds=time.time()-start,shared_probe=True,candidate_count=len(cases),scope='本次串行编译、运行、定义提取；不冒充逐项人工审读用时'))
print('structural probes completed:',len(cases),'candidates, two libraries, five repetitions each')
