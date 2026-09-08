#!/usr/bin/env python3
"""完整修改库的轻量原生验证：四等待及新头实例，每格五轮。"""
import pathlib,subprocess,sys,csv
r=pathlib.Path.cwd();out=r/'tmp/IMPL_0908/w1-probes';out.mkdir(exist_ok=True)
build=r/'tmp/IMPL_0908/build-native';lib=build/'lib';raw=r/('docs/progress/IMPL_0908/W1/raw/smoke_'+sys.argv[1]);i=0
compiler='/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++'
def run(label,args):
 global i;i+=1;stem=raw/f'{i:03}_{label}'
 rc=subprocess.run([sys.executable,'docs/progress/R114/code/record.py',str(stem),'nice','-n','15','ionice','-c','3',*map(str,args)]).returncode
 assert rc==0,(label,rc)
 return pathlib.Path(str(stem)+'.stdout').read_text()
common=['-std=c++20','-O0','-g','-nostdinc++','-I',build/'include/c++/v1']
link=['-nostdlib++','-L',lib,'-Wl,-rpath-link,'+str(lib),'-lc++','-lc++abi','-latomic','-pthread','-ldl']
run('identity',[compiler,*common,'-c',r/'docs/progress/IMPL_0908/code/runtime_identity.cpp','-o',out/'identity.o'])
for name,source in [('wait',r/'docs/progress/R111/code/condition_cancel_probe.cpp'),('headers',r/'docs/progress/IMPL_0908/code/header_smoke.cpp')]:
 run('build_'+name,[compiler,*common,'-Wno-deprecated-declarations',source,out/'identity.o',*link,'-o',out/name])
rows=[]
for name,mode in [('wait',x) for x in ['wait','system_clock','steady_clock','custom_clock']]+[('headers','')]:
 for repeat in range(1,6):
  output=run(name+'_'+mode+str(repeat),['timeout','35','env','LD_LIBRARY_PATH='+str(lib),'EXPECTED_RUNTIME='+str(lib),out/name,*([mode] if mode else [])])
  assert 'RUNTIME_IDENTITY=PASS' in output
  assert ('canceled=1 cleanup_count=1' in output) if name=='wait' else ('HEADER_VALUE_ASSERTIONS=PASS' in output)
  rows.append([name,mode,repeat,'PASS'])
with (r/'docs/progress/IMPL_0908/W1/smoke.tsv').open('w') as f:
 w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['probe','mode','repeat','values']);w.writerows(rows)
print('W1_VALUE_SMOKE=PASS runs='+str(len(rows)))
