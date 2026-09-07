#!/usr/bin/env python3
import pathlib,subprocess,sys
r=pathlib.Path.cwd();task=r/'tmp/R116';platform=pathlib.Path('/home/toolchain/development/libc++_replacement');out=task/'probes';out.mkdir(exist_ok=True)
cxx=platform/'progress/R33/tools/tizen-clang++';headers=platform/'tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1';libs=platform/'tmp/R69/build-x86_64/lib';i=0
def run(label,args):
 global i;i+=1
 subprocess.run([sys.executable,'docs/progress/R114/code/record.py',f'docs/progress/R116/raw/build_probes/{i:03}_{label}',
 'nice','-n','15','ionice','-c','3',*map(str,args)],check=True)
common=['-std=c++20','-O0','-g','-fPIC','-nostdinc++']
link=['-nostdlib++','-L',libs,'-Wl,-rpath-link,'+str(libs),'-lc++','-lc++abi','-pthread','-ldl','-latomic','-Wl,-export-dynamic']
run('identity',[cxx,*common,'-I',headers,'-c',r/'docs/progress/R116/code/identity.cpp','-o',out/'identity.o'])
for variant in ['control','rollback']:
 dest=out/variant;dest.mkdir(exist_ok=True)
 run('copy_'+variant,['cp',r/f'tmp/R115-W4/x86_64/functional/{variant}/libw4.so',dest/'libw4.so'])
 run('shared_'+variant,[cxx,*common,'-DW4_TRACE','-fno-access-control','-include',r/'tmp/R115-W4/trace.h','-I',r/f'tmp/R115-W4/{variant}/include','-I',headers,
 r/'tmp/R115-W4/probe.cpp',out/'identity.o','-L',dest,'-Wl,--no-as-needed','-lw4',*link,'-o',dest/'shared'])
for spec in ['retained','removed']:
 inc=['-I',r/'tmp/R115-W4/control/include'] if spec=='removed' else []
 extra=['-L',out/'control','-Wl,--no-as-needed','-lw4'] if spec=='removed' else []
 run('wait_'+spec,[cxx,*common,*inc,'-I',headers,r/'docs/progress/R111/code/condition_cancel_probe.cpp',out/'identity.o',*extra,*link,'-o',out/('wait_'+spec)])
run('plugin',[cxx,*common,'-I',headers,'-shared',r/'docs/progress/R116/code/plugin.cpp',*link,'-o',out/'libr116_plugin.so'])
run('api',[cxx,*common,'-I',headers,r/'docs/progress/R116/code/api_probe.cpp',out/'identity.o',*link,'-o',out/'api'])
(task/'abi-unpatched').mkdir(exist_ok=True)
run('unpatched_abi',['cp','-L',platform/'tmp/R68/build-baseline-x86_64/lib/libc++abi.so.1',task/'abi-unpatched/libc++abi.so.1'])
run('hashes',['sha256sum',*sorted(out.glob('wait_*')),out/'api',out/'libr116_plugin.so',task/'abi-unpatched/libc++abi.so.1'])
