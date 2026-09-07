#!/usr/bin/env python3
import pathlib,subprocess,sys
arch=sys.argv[1];base=pathlib.Path('/home/toolchain/development/libc++_replacement')
out=pathlib.Path.cwd()/'tmp/R115-W3'/arch
cc=base/('progress/R36/tools/armv7l-clang++' if arch=='armv7l' else 'progress/R33/tools/tizen-clang++')
headers=base/f'tmp/R68/build-baseline-{arch}/libcxx/test-suite-install/include/c++/v1';rt=base/f'tmp/R69/build-{arch}/lib'
i=0
def run(label,args):
 global i
 i+=1
 subprocess.run([sys.executable,'docs/progress/R114/code/record.py',f'docs/progress/R115/W3/raw/supplement_{arch}/{i:03}_{label}','nice','-n','15','ionice','-c','3',*map(str,args)],check=True)
run('deadline',[cc,'-c','docs/progress/R115/code/deadline.cpp','-o',out/'supplement_deadline.o'])
for name in ['old_retained','new_noisy','new_quiet']:
 obj=out/'old_noisy.o'
 if name!='old_retained':
  obj=out/(name+'.o')
  run(name+'_compile',[cc,'-std=c++20','-O0','-g','-fno-inline','-fPIC','-nostdinc++','-I',out/'overlay/include','-I',headers,'-DEXPECT_CLEAN=1',*(['-DWORKER_NO_PUTS'] if name=='new_quiet' else []),'-c','docs/progress/R112/code/item1_oldcaller_cancel.cpp','-o',obj])
 extra=[] if name=='old_retained' else ['-L',out,'-Wl,--no-as-needed','-lremoved_r115']
 run(name+'_link',[cc,obj,out/'supplement_deadline.o',*extra,'-nostdlib++','-L',rt,'-Wl,-rpath-link,'+str(rt),'-lc++','-lc++abi','-latomic','-pthread','-ldl','-o',out/(name+'_bounded')])
