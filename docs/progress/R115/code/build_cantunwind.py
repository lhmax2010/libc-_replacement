#!/usr/bin/env python3
import pathlib,subprocess,sys
base=pathlib.Path('/home/toolchain/development/libc++_replacement')
out=pathlib.Path.cwd()/'tmp/R115-W3/armv7l'
compiler=base/'progress/R36/tools/armv7l-clang++'
runtime=base/'tmp/R69/build-armv7l/lib'
i=0
def run(label,args):
 global i
 i+=1
 subprocess.run([sys.executable,'docs/progress/R114/code/record.py',f'docs/progress/R115/W3/raw/cant_build/{i:03}_{label}','nice','-n','15','ionice','-c','3',*map(str,args)],check=True)
run('gcc_assemble',[compiler,'-c',out/'cantunwind_gcc.s','-o',out/'cantunwind_gcc.o'])
run('clang_compile',[compiler,'-O0','-fno-exceptions','-c','docs/progress/R115/code/cantunwind_worker.cpp','-o',out/'cantunwind_clang.o'])
run('main',[compiler,'-c','docs/progress/R115/code/cantunwind_main.cpp','-o',out/'cantunwind_main.o'])
for name in ['gcc','clang']:
 run(name+'_link',[compiler,out/'cantunwind_main.o',out/f'cantunwind_{name}.o','-nostdlib++','-Wl,--no-as-needed','-L',runtime,'-Wl,-rpath-link,'+str(runtime),'-lc++','-lc++abi','-latomic','-pthread','-ldl','-o',out/f'cantunwind_{name}'])
 run(name+'_exidx',['readelf','-uW',out/f'cantunwind_{name}'])
run('hashes',['sha256sum',out/'cantunwind_gcc',out/'cantunwind_clang'])
