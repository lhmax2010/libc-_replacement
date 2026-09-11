#!/usr/bin/env python3
import pathlib,subprocess,sys,json
r=pathlib.Path.cwd();o=r/'docs/progress/API_0911/W1_CONT';t=r/'tmp/API_0911/W1_CONT';b=r/'tmp/IMPL_0908/build-native'
def run(stem,cmd):
 return subprocess.run([sys.executable,str(r/'docs/progress/API_0911/record.py'),str(o/'raw'/stem),'nice','-n','15','ionice','-c','3',*cmd]).returncode
rows=[]
for kind in ['gnu','cxx']:
 cc=['g++'] if kind=='gnu' else ['/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++','-nostdinc++','-I'+str(b/'include/c++/v1'),'-nostdlib++']
 cmd=cc+['-std=c++17','-O0','-g',str(o/'vector_representation.cpp'),'-L'+str(b/'lib'),'-Wl,--no-as-needed','-lc++','-lc++abi','-lstdc++','-ldl','-o',str(t/('vector_repr_'+kind))]
 if run('vector_repr_build_'+kind,cmd):continue
 for repeat in range(1,6):
  stem=f'vector_repr_{kind}_{repeat}'
  rc=run(stem,['prlimit','--core=0','--','timeout','30',str(t/'closure_ewk/ld-linux-x86-64.so.2'),'--library-path',str(t/'closure_ewk')+':'+str(b/'lib'),str(t/('vector_repr_'+kind)),str(r/'tmp/API_0911/W1/chromium_image/lib/libchromium-impl.so')])
  out=(o/'raw'/(stem+'.stdout')).read_text(errors='replace')
  rows.append(dict(library=kind,repeat=repeat,returncode=rc,maps='MAP_ASSERT cxx=1 gnu=1 provider=1' in out,representation=out.count('three_pointer_match=1')==4,exact_and_destroy='REPRESENTATION_AND_OWN_LIBRARY_COPY_DESTROY_PASS cases=4' in out,actual_SetSyncBinaryReply_called=False,raw=stem))
(o/'vector_representation_results.json').write_text(json.dumps(rows,indent=2)+'\n')
