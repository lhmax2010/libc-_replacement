#!/usr/bin/env python3
import pathlib,subprocess,sys,json,hashlib
r=pathlib.Path.cwd();o=r/'docs/progress/API_0911/W1_CONT';t=r/'tmp/API_0911/W1_CONT';b=r/'tmp/IMPL_0908/build-native'
includes=sorted({str(pathlib.Path(x['path']).parent) for x in json.loads((o/'ewk_header_identity.json').read_text())})
inc=['-I'+p for p in includes]
def run(stem,cmd):return subprocess.run([sys.executable,str(r/'docs/progress/API_0911/record.py'),str(o/'raw'/stem),'nice','-n','15','ionice','-c','3',*cmd]).returncode
cxx=['/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++','-nostdinc++','-I'+str(b/'include/c++/v1'),'-nostdlib++']
common=['-std=c++17','-O0','-g','-fPIC','-fvisibility=hidden',*inc]
steps=[('ewk_shim_gnu_compile',['g++',*common,'-c',str(o/'ewk_bridge_gnu.cpp'),'-o',str(t/'ewk_bridge_gnu.o')]),('ewk_shim_cxx_compile',[*cxx,*common,'-c',str(o/'ewk_bridge_cxx.cpp'),'-o',str(t/'ewk_bridge_cxx.o')]),('ewk_shim_link',['gcc','-shared',str(t/'ewk_bridge_gnu.o'),str(t/'ewk_bridge_cxx.o'),'-Wl,-z,defs','-L'+str(b/'lib'),'-Wl,--no-as-needed','-lc++','-lc++abi','-lstdc++','-ldl','-o',str(t/'ewk_shim.so')]),('ewk_shim_probe_build',[*cxx,'-std=c++17','-O0','-g',*inc,str(o/'ewk_shim_probe.cpp'),'-L'+str(b/'lib'),'-lc++','-lc++abi','-ldl','-o',str(t/'ewk_shim_probe')])]
for stem,cmd in steps:
 if run(stem,cmd):raise SystemExit(1)
rows=[]
for repeat in range(1,6):
 stem=f'ewk_real_shim_cxx_to_gnu_{repeat}'
 rc=run(stem,['prlimit','--core=0','--','timeout','30',str(t/'closure_ewk/ld-linux-x86-64.so.2'),'--library-path',str(t/'closure_ewk')+':'+str(b/'lib'),str(t/'ewk_shim_probe'),str(t/'ewk_shim.so'),str(r/'tmp/API_0911/W1/chromium_image/lib/libchromium-impl.so')])
 out=(o/'raw'/(stem+'.stdout')).read_text(errors='replace')
 rows.append(dict(repeat=repeat,consumer='cxx',real_provider='gnu',returncode=rc,maps='MAP_ASSERT cxx=1 gnu=1 shim=1 provider=1' in out,exact='EXACT_FIELDS=1' in out,destroyed='SHIM_CONSUMER_DESTROYED=1' in out,raw=stem))
(o/'ewk_shim_results.json').write_text(json.dumps(rows,indent=2)+'\n')
