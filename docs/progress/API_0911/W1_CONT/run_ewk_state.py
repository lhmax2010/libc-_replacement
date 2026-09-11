#!/usr/bin/env python3
import pathlib,subprocess,sys,json
r=pathlib.Path.cwd();o=r/'docs/progress/API_0911/W1_CONT';t=r/'tmp/API_0911/W1_CONT';b=r/'tmp/IMPL_0908/build-native';provider=r/'tmp/API_0911/W1/chromium_image/lib/libchromium-impl.so'
inc=['-I'+p for p in sorted({str(pathlib.Path(x['path']).parent) for x in json.loads((o/'ewk_header_identity.json').read_text())})]
def run(stem,cmd):return subprocess.run([sys.executable,str(r/'docs/progress/API_0911/record.py'),str(o/'raw'/stem),'nice','-n','15','ionice','-c','3',*cmd]).returncode
rows=[]
for kind in ['gnu','cxx']:
 cc=['g++'] if kind=='gnu' else ['/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++','-nostdinc++','-I'+str(b/'include/c++/v1'),'-nostdlib++']
 assert run('ewk_state_build_'+kind,[*cc,'-std=c++17','-O0','-g',*inc,str(o/'ewk_state_probe.cpp'),'-L'+str(b/'lib'),'-Wl,--no-as-needed','-lc++','-lc++abi','-lstdc++','-ldl','-o',str(t/('ewk_state_'+kind))])==0
 for repeat in range(1,6):
  stem=f'ewk_reused_output_{kind}_{repeat}';entry=provider if kind=='gnu' else t/'ewk_shim.so'
  rc=run(stem,['prlimit','--core=0','--','timeout','30',str(t/'closure_ewk/ld-linux-x86-64.so.2'),'--library-path',str(t/'closure_ewk')+':'+str(b/'lib'),str(t/('ewk_state_'+kind)),str(entry),str(provider)])
  out=(o/'raw'/(stem+'.stdout')).read_text(errors='replace')
  rows.append(dict(consumer=kind,path='direct' if kind=='gnu' else 'experimental_shim',repeat=repeat,returncode=rc,maps='MAP_ASSERT cxx=1 gnu=1 entry=1 provider=1' in out,matches_direct='REUSED_OUTPUT_MATCHES_DIRECT=1' in out,destroyed='REUSED_OUTPUT_DESTROYED=1' in out,raw=stem))
(o/'ewk_reused_output_results.json').write_text(json.dumps(rows,indent=2)+'\n')
