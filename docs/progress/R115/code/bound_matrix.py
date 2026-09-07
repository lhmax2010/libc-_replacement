#!/usr/bin/env python3
import pathlib,subprocess,sys
arch=sys.argv[1]
base=pathlib.Path('/home/toolchain/development/libc++_replacement')
out=pathlib.Path.cwd()/'tmp/R115-W3'/arch
runtime=base/f'tmp/R69/build-{arch}/lib'
compiler=base/('progress/R36/tools/armv7l-clang++' if arch=='armv7l' else 'progress/R33/tools/tizen-clang++')
i=0
def run(label,args):
 global i
 i+=1
 subprocess.run([sys.executable,'docs/progress/R114/code/record.py',f'docs/progress/R115/W3/raw/bound_{arch}/{i:03}_{label}','nice','-n','15','ionice','-c','3',*map(str,args)],check=True)
run('compile',[compiler,'-c','docs/progress/R115/code/deadline.cpp','-o',out/'deadline.o'])
for name in ['wait_retained','wait_removed','facility','old_noisy','old_quiet','old_fno','old_fno_quiet']:
 extra=[] if name=='wait_retained' else ['-L',out,'-Wl,--no-as-needed','-lremoved_r115']
 run(name,[compiler,out/(name+'.o'),out/'deadline.o',*extra,'-nostdlib++','-L',runtime,'-Wl,-rpath-link,'+str(runtime),'-lc++','-lc++abi','-latomic','-pthread','-ldl','-Wl,-export-dynamic','-o',out/(name+'_bounded')])
