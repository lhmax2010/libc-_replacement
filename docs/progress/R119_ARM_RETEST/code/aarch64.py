#!/usr/bin/env python3
from common import *
from run_types import CC,TMP
import re
gate('aarch64_gate')
root=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/R105-boost-aarch64/local/BUILD-ROOTS/scratch.aarch64.0')
gnu=root/'usr/lib64/gcc/aarch64-tizen-linux-gnu/14.2.0/include/c++'
qemu=pathlib.Path('/usr/bin/qemu-arm64-static');src=OUT/'probes/types.cpp'
rows=list(csv.DictReader((OUT/'ARMV7L_TYPES.tsv').open(),delimiter='\t'));assert len(rows)==13
allvalues={};configs=[]
for lib in ['gnu','cxx']:
 flags=['--target=aarch64-tizen-linux-gnu','--sysroot='+str(root),'--ld-path='+str(OUT/'code/aarch64-ld.sh'),'-std=c++17','-O0','-g','-fexceptions','-nostdinc++','-nostdlib++','-Wno-deprecated-declarations']
 if lib=='gnu':flags+=['-I'+str(gnu),'-I'+str(gnu/'aarch64-tizen-linux-gnu')];links=[str(root/'usr/lib64/libstdc++.so.6')]
 else:flags+=['-I'+str(root/'usr/include/c++/v1')];links=['-L'+str(root/'usr/lib64'),'-lc++','-lc++abi']
 exe=TMP/('aarch64_'+lib)
 rc,out,build=record('aarch64_build_'+lib,[CC,*flags,src,*links,'-pthread','-o',exe]);assert rc==0,build
 rc,out,symref=record('aarch64_symbols_'+lib,['readelf','-h','-Ws','--wide',exe]);assert rc==0 and 'AArch64' in out
 rc,out,macroref=record('aarch64_macros_'+lib,[CC,*flags,'-dM','-E',src]);assert rc==0
 assert ('#define _GLIBCXX_RELEASE 14' if lib=='gnu' else '#define _LIBCPP_VERSION 220108') in out
 rc,out,incref=record('aarch64_includes_'+lib,[CC,*flags,'-H','-fsyntax-only',src]);assert rc==0
 headers={}
 for line in (ROOT/(incref+'.stderr')).read_text().splitlines():
  m=re.match(r'^\.+ (/.+)$',line)
  if m and pathlib.Path(m[1]).is_file():headers[m[1]]=sha(m[1])
 runs=[]
 for n in range(1,6):
  rc,out,ref=record(f'aarch64_run_{lib}_{n}',['timeout','30',qemu,'-L',root,root/'lib64/ld-linux-aarch64.so.1','--library-path',str(root/'lib64')+':'+str(root/'usr/lib64'),exe]);assert rc==0
  assert 'TARGET_POINTER=8 TARGET_LONG=8 TARGET_LONG_LONG=8' in out
  values={s.split('\t')[0]:s for s in out.splitlines() if '\t' in s}
  for name,code in [('INT','i'),('LONG','l'),('LONG_LONG','x'),('CONST_REF','RKl')]:assert 'TOKEN=9TypeTokenI'+code+'E\t' in values['CONTROL_'+name]
  assert set(r['id'] for r in rows)<=set(values);runs.append((values,ref))
 for r in rows:
  assert len({v[r['id']] for v,_ in runs})==1
  line=runs[0][0][r['id']];m=dict(id=r['id'],expression=r['expression'],library=lib,architecture='aarch64',environment='QEMU_USER',actual_type=re.search(r'TYPE=.*?\[T = (.*?)\]',line)[1],token=re.search(r'TOKEN=([^\t]+)',line)[1],direct=re.search(r'DIRECT=([^\t]+)',line)[1],size=int(re.search(r'SIZE=(\d+)',line)[1]),align=int(re.search(r'ALIGN=(\d+)',line)[1]),output=line,runs=[ref for _,ref in runs],repeat_count=5,build=build,symbols=symref,source=str(src.relative_to(ROOT)),source_sha256=sha(src),executable_sha256=sha(exe))
  save(OUT/'aarch64_measurements'/f'{r["id"]}_{lib}.json',m);allvalues[r['id'],lib]=m
 configs.append(dict(library=lib,target='aarch64-tizen-linux-gnu',sysroot=str(root),flags=flags,links=links,macros=macroref,headers=headers,qemu=str(qemu),qemu_sha256=sha(qemu)))
table=[]
for r in rows:
 a,b=allvalues[r['id'],'gnu'],allvalues[r['id'],'cxx'];table.append(dict(id=r['id'],expression=r['expression'],gnu_type=a['actual_type'],cxx_type=b['actual_type'],gnu_token=a['token'],cxx_token=b['token'],gnu_direct=a['direct'],cxx_direct=b['direct'],gnu_size=a['size'],cxx_size=b['size'],gnu_align=a['align'],cxx_align=b['align'],result='IDENTICAL' if a['token']==b['token'] else 'DIVERGENT'))
tsv(OUT/'AARCH64_TYPES.tsv',table);save(OUT/'AARCH64_CONFIGURATION.json',configs)
print(json.dumps(table,indent=2))
