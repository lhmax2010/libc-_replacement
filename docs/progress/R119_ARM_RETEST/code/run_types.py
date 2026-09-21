#!/usr/bin/env python3
"""复用已验证 source()/PRE；仅选择指定13项，不重新实现类型探针。"""
from common import *
import ast,re,os
OLD=pathlib.Path('/home/toolchain/development/libc++_replacement')
CC=OLD/'progress/R33/tools/tizen-clang++'
ARMROOT=OLD/'tmp/GBS-ROOT/R30-semantic-libcxx-armv7l-20260812-01/local/BUILD-ROOTS/scratch.armv7l.0'
SDK=ROOT/'tmp/API_0911/W1_CONT/arm/sdk'
BUILD=ROOT/'tmp/IMPL_0908/build-armv7l'
GNU=SDK/'libstdc++-devel/usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/c++'
TMP=ROOT/'tmp/R119_ARM_RETEST';TMP.mkdir(parents=True,exist_ok=True)
QEMU=pathlib.Path('/usr/bin/qemu-arm-static')
def configuration(lib):
 opts=['--target=armv7l-tizen-linux-gnueabi','--sysroot='+str(ARMROOT),'--ld-path='+str(OLD/'progress/R36/tools/armv7l-ld'),'-march=armv7-a','-mfpu=neon','-mfloat-abi=softfp','-mthumb','-std=c++17','-O0','-g','-fexceptions','-Wno-deprecated-declarations','-nostdinc++','-nostdlib++']
 if lib=='gnu':return opts+['-I'+str(GNU),'-I'+str(GNU/'armv7l-tizen-linux-gnueabi')],[str(SDK/'libstdc++/usr/lib/libstdc++.so.6')]
 return opts+['-I'+str(BUILD/'include/c++/v1')],['-L'+str(BUILD/'lib'),'-lc++','-lc++abi']
def original_source():
 p=PRIOR/'code/measure.py';text=p.read_text();tree=ast.parse(text)
 nodes=[n for n in tree.body if (isinstance(n,ast.Assign) and any(isinstance(t,ast.Name) and t.id=='PRE' for t in n.targets)) or (isinstance(n,ast.FunctionDef) and n.name=='source')]
 assert len(nodes)==2;ns={'re':re};exec(compile(ast.Module(body=nodes,type_ignores=[]),str(p),'exec'),ns)
 save(OUT/'PROBE_REUSE.json',dict(path=str(p),sha256=sha(p),functions=['PRE','source'],method='AST 仅提取原生成器；不执行旧脚本的目录/运行副作用',adaptation='main 开头增加库版本与目标宏输出；类型/编码/sizeof/alignof emit 未修改'))
 return ns['source']
def main():
 gate('types_gate')
 frozen={str(p.relative_to(ROOT)):sha(p) for p in PRIOR.rglob('*') if p.is_file() and '__pycache__' not in p.parts};save(OUT/'PRIOR_FROZEN_SHA256.json',frozen)
 rows=list(csv.DictReader((PRIOR/'BUILTIN_DIVERGENT.tsv').open(),delimiter='\t'))
 expected=['T0001','T0014','T0019','T0024','T0029','T0356','T0365','T0369','T0373','T0378','T0380','T0382','T0384']
 assert [r['id'] for r in rows]==expected and len(rows)==13
 src=OUT/'probes/types.cpp';src.parent.mkdir(parents=True,exist_ok=True)
 code=original_source()(rows)
 extra='''
#ifdef _LIBCPP_VERSION
 std::printf("LIBRARY=libc++ VERSION=%d\\n",_LIBCPP_VERSION);
#else
 std::printf("LIBRARY=libstdc++ RELEASE=%d\\n",_GLIBCXX_RELEASE);
#endif
 std::printf("TARGET_POINTER=%zu TARGET_LONG=%zu TARGET_LONG_LONG=%zu\\n",sizeof(void*),sizeof(long),sizeof(long long));
'''
 code=code.replace('int main(){','int main(){'+extra,1);src.write_text(code)
 result={};libraries=[]
 for lib in ['gnu','cxx']:
  opts,links=configuration(lib);exe=TMP/('types_'+lib)
  rc,out,build=record('build_'+lib,[CC,*opts,src,*links,'-pthread','-o',exe]);assert rc==0,build
  rc,out,symref=record('symbols_'+lib,['readelf','-h','-Ws','-d','--wide',exe]);assert rc==0 and 'Machine:' in out and 'ARM' in out
  rc,macros,macroref=record('macros_'+lib,[CC,*opts,'-dM','-E',src]);assert rc==0
  rc,inc,incref=record('includes_'+lib,[CC,*opts,'-H','-fsyntax-only',src]);assert rc==0
  includes=[]
  for line in (ROOT/(incref+'.stderr')).read_text().splitlines():
   m=re.match(r'^\.+ (/.+)$',line)
   if m and pathlib.Path(m[1]).is_file():includes.append(dict(path=m[1],sha256=sha(m[1])))
  allruns=[]
  for n in range(1,6):
   loader=ARMROOT/'lib/ld-linux.so.3'
   paths=':'.join(map(str,[BUILD/'lib',SDK/'libstdc++/usr/lib',ARMROOT/'lib',ARMROOT/'usr/lib']))
   rc,out,ref=record(f'run_{lib}_{n}',['timeout','30',QEMU,'-L',ARMROOT,loader,'--library-path',paths,exe]);assert rc==0,ref
   assert ('LIBRARY=libstdc++ RELEASE=14' if lib=='gnu' else 'LIBRARY=libc++ VERSION=220108') in out
   assert 'TARGET_POINTER=4 TARGET_LONG=4 TARGET_LONG_LONG=8' in out
   values={s.split('\t')[0]:s for s in out.splitlines() if '\t' in s}
   for name,encoding in [('INT','i'),('LONG','l'),('LONG_LONG','x'),('CONST_REF','RKl')]:assert 'TOKEN=9TypeTokenI'+encoding+'E\t' in values['CONTROL_'+name]
   assert set(expected)<=set(values);allruns.append((values,ref))
  for r in rows:
   outputs=[v[r['id']] for v,ref in allruns];assert len(set(outputs))==1
   line=outputs[0];assert 'UNAVAILABLE' not in line
   data=dict(id=r['id'],expression=r['expression'],library=lib,architecture='armv7l',environment='QEMU_USER',actual_type=re.search(r'TYPE=.*?\[T = (.*?)\]',line)[1],token=re.search(r'TOKEN=([^\t]+)',line)[1],direct=re.search(r'DIRECT=([^\t]+)',line)[1],size=int(re.search(r'SIZE=(\d+)',line)[1]),align=int(re.search(r'ALIGN=(\d+)',line)[1]),output=line,source=str(src.relative_to(ROOT)),source_sha256=sha(src),executable_sha256=sha(exe),build=build,symbols=symref,runs=[ref for v,ref in allruns],repeat_count=5)
   save(OUT/'measurements'/f'{r["id"]}_{lib}.json',data);result[r['id'],lib]=data
  libraries.append(dict(library=lib,compiler=str(CC),flags=opts,links=links,sysroot=str(ARMROOT),target='armv7l-tizen-linux-gnueabi',macro_record=macroref,header_record=incref,includes=includes,emulator=str(QEMU),emulator_sha256=sha(QEMU)))
 table=[]
 for r in rows:
  a,b=result[r['id'],'gnu'],result[r['id'],'cxx']
  table.append(dict(id=r['id'],expression=r['expression'],gnu_type=a['actual_type'],cxx_type=b['actual_type'],gnu_token=a['token'],cxx_token=b['token'],gnu_direct=a['direct'],cxx_direct=b['direct'],gnu_size=a['size'],cxx_size=b['size'],gnu_align=a['align'],cxx_align=b['align'],result='IDENTICAL' if a['token']==b['token'] else 'DIVERGENT',gnu_record=f'measurements/{r["id"]}_gnu.json',cxx_record=f'measurements/{r["id"]}_cxx.json'))
 tsv(OUT/'ARMV7L_TYPES.tsv',table);save(OUT/'CONFIGURATION.json',libraries)
 for p,digest in frozen.items():assert sha(ROOT/p)==digest,p
 print(json.dumps(table,ensure_ascii=False,indent=2))
if __name__=='__main__':main()
