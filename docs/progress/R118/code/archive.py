#!/usr/bin/env python3
"""生成身份、源码定位及可直接比对的完整数值附录；不推断缺项。"""
import pathlib,json,hashlib,gzip,csv,re,sys
from run import ROOT,OUT,CODE,configuration,digest,save
typ=sys.argv[1]
paths={'string':['codes/llvm/libcxx/include/string','codes/llvm/libcxx/include/__string/char_traits.h','codes/llvm/libcxx/include/__functional/hash.h','codes/gcc/libstdc++-v3/include/bits/basic_string.h','codes/gcc/libstdc++-v3/include/bits/basic_string.tcc','codes/gcc/libstdc++-v3/include/bits/char_traits.h','codes/gcc/libstdc++-v3/include/ext/vstring.h'],'vector':['codes/llvm/libcxx/include/__vector/vector.h','codes/llvm/libcxx/include/__vector/vector_bool.h','codes/gcc/libstdc++-v3/include/bits/stl_vector.h','codes/gcc/libstdc++-v3/include/bits/vector.tcc','codes/gcc/libstdc++-v3/include/bits/stl_bvector.h','codes/gcc/libstdc++-v3/include/debug/vector'],'function':['codes/llvm/libcxx/include/__functional/function.h','codes/gcc/libstdc++-v3/include/bits/std_function.h']}[typ]
paths += {'string':['codes/gcc/libstdc++-v3/libsupc++/hash_bytes.cc'], 'vector':[], 'function':['codes/llvm/libcxx/src/functional.cpp','codes/gcc/libstdc++-v3/src/c++11/functional.cc']}[typ]
sources=[]
for name in paths:
 p=ROOT/name;dest=OUT/typ/'source'/pathlib.Path(name).name
 dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(p.read_bytes());sources.append(dict(path=name,sha256=digest(p),snapshot=str(dest.relative_to(OUT))))
save(OUT/typ/'sources.json',sources)
ident=[]
for arch in ['x86_64','armv7l']:
 for lib in ['gnu','cxx']:
  flags,links,b,g=configuration(arch,lib)
  for p in [b/'lib/libc++.so.1.0',b/'lib/libc++abi.so.1.0',g,b/'include/c++/v1/__config_site']:
   ident.append(dict(arch=arch,path=str(p),sha256=digest(p)))
save(OUT/typ/'identities.json',ident)
lines=['# '+typ+' 完整数值附录','','每段为该格第 1 次；全部五次在对应 JSON 与 raw 中。MAP 地址仅从此比对附录剔除，原始输出完整保留。','']
for f in sorted((OUT/typ).glob('*matrix.json')):
 for row in json.loads(f.read_text()):
  lines += ['## '+row['arch']+' / '+row['library']+' / '+row['test'],'',f"编译退出码：{row['build_exit']}；[原始命令](../{row['build_raw'].split('R118/')[1]}.command.txt)",'']
  if row['runs']:lines+=['```text',row['runs'][0]['result'],'```','',f"重复次数：{len(row['runs'])}；数值重复一致：{row.get('repeat_results_identical','NOT_OBSERVED')}。",'']
(OUT/typ/'VALUES.md').write_text('\n'.join(lines))
# R117 类型及成员逐项抽取。单侧未解析不是另一侧缺失的证明。
with gzip.open(ROOT/'docs/progress/R117/public_members.tsv.gz','rt') as f:
 reader=csv.DictReader(f,delimiter='\t');fields=reader.fieldnames;matches=[r for r in reader if any(typ in r.get(k,'') for k in ['owner','entity'])]
with (OUT/typ/'R117_members.tsv').open('w') as f:
 w=csv.DictWriter(f,fieldnames=fields,delimiter='\t');w.writeheader();w.writerows(matches)
with (ROOT/'docs/progress/R117/SKELETON.tsv').open() as f:
 reader=csv.DictReader(f,delimiter='\t');fields=reader.fieldnames;rows=[r for r in reader if any(typ in v for k,v in r.items() if k in ['entity','类型/函数'])]
with (OUT/typ/'R117_entities.tsv').open('w') as f:
 w=csv.DictWriter(f,fieldnames=fields,delimiter='\t');w.writeheader();w.writerows(rows)
manifest=[dict(path=str(p.relative_to(OUT)),sha256=digest(p))for p in sorted(OUT.rglob('*'))if p.is_file() and p.name not in ['SHA256.json'] and '__pycache__' not in str(p)]
save(OUT/'SHA256.json',manifest)
print(json.dumps(dict(type=typ,source_files=len(sources),member_rows=len(matches),entity_rows=len(rows),manifest_files=len(manifest))))
