#!/usr/bin/env python3
from common import *
import re,collections
gate('refine_gate');rows=json.loads((OUT/'TYPE_RESULTS.json').read_text())
codes=sorted({r[lib+'_encoding'] for r in rows if r['result']!='NOT_AVAILABLE' for lib in ['gnu','cxx']});decoded={}
for i in range(0,len(codes),60):
 rc,out,ref=record('decode_'+str(i),['c++filt','-t',*codes[i:i+60]]);assert rc==0
 # resource wrapper adds two diagnostic lines before c++filt output.
 lines=[s for s in out.splitlines() if not s.startswith(('memory_basis=','RLIMIT_AS_bytes='))]
 assert len(lines)==len(codes[i:i+60]);decoded.update(zip(codes[i:i+60],lines))
def norm(s):return s.replace('std::__1::','std::').replace('std::__cxx11::','std::').replace('std::filesystem::__cxx11::','std::filesystem::')
for r in rows:
 if r['result']=='NOT_AVAILABLE':r['mechanism']='请求类型不可得';continue
 a,b=[decoded[r[lib+'_encoding']] for lib in ['gnu','cxx']];r['gnu_canonical']=a;r['cxx_canonical']=b
 if r['result']=='IDENTICAL':r['mechanism']='类型编码相同（不证明类布局相同）'
 elif norm(a)==norm(b):r['mechanism']='仅内联命名空间/ABI标签导致编码不同'
 elif re.fullmatch(r'[PKROVr]*[a-z]',r['gnu_encoding']) and re.fullmatch(r'[PKROVr]*[a-z]',r['cxx_encoding']):r['mechanism']='内建底层类型选择不同'
 elif r['family']=='ios' and r['expression'].startswith('std::ios_base::'):r['mechanism']='枚举/内建类型或枚举身份不同'
 else:r['mechanism']='实现类/模板实参/组合类型不同'
tsv(OUT/'TYPE_RESULTS.tsv',[{k:r.get(k,'NOT_OBSERVED') for k in list(rows[0])} for r in rows]);save(OUT/'TYPE_RESULTS.json',rows)
div=[r for r in rows if r['result']=='DIVERGENT'];tsv(OUT/'DIVERGENT_TYPES.tsv',div)
save(OUT/'MECHANISM_COUNTS.json',dict(collections.Counter(r['mechanism'] for r in div)))
print('DIVERGENT mechanisms',dict(collections.Counter(r['mechanism'] for r in div)))
print('scalar choices',[(r['expression'],r['gnu_canonical'],r['cxx_canonical']) for r in div if r['mechanism']=='内建底层类型选择不同'])
