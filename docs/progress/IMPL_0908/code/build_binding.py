#!/usr/bin/env python3
"""完整单/双版本库的四绑定矩阵构建；旧 quiet.o 与新头对象独立编译。"""
import pathlib,subprocess,sys
r=pathlib.Path.cwd();out=r/'tmp/IMPL_0908/binding-native';out.mkdir(exist_ok=True)
old=r/'tmp/IMPL_0908/build-baseline-native';new=r/'tmp/IMPL_0908/build-native';i=0
compiler='/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++'
def run(label,args):
 global i;i+=1
 subprocess.run([sys.executable,'docs/progress/R114/code/record.py',f'docs/progress/IMPL_0908/W2/raw/binding_build/{i:03}_{label}',
 'nice','-n','15','ionice','-c','3',*map(str,args)],check=True)
common=['-std=c++20','-O0','-g','-fno-inline','-fPIC','-nostdinc++']
run('identity',[compiler,*common,'-I',new/'include/c++/v1','-c',r/'docs/progress/IMPL_0908/code/runtime_identity.cpp','-o',out/'identity.o'])
for name,build,clean in [('old',old,0),('new',new,1)]:
 run('compile_'+name,[compiler,*common,'-I',build/'include/c++/v1','-DWORKER_NO_PUTS','-DEXPECT_CLEAN='+str(clean),
                     '-c',r/'docs/progress/R112/code/item1_oldcaller_cancel.cpp','-o',out/(name+'.o')])
for name,obj,lib in [('old_elf','old',old),('old_relinked','old',new),('new_object','new',new),('new_prelinked_old','new',old)]:
 run('link_'+name,[compiler,out/(obj+'.o'),out/'identity.o','-nostdlib++','-L',lib/'lib',
      '-Wl,-rpath-link,'+str(lib/'lib'),'-lc++','-lc++abi','-latomic','-pthread','-ldl','-o',out/name])
 run('bindings_'+name,['readelf','--dyn-syms','--wide',out/name])
for name,build in [('timed_old',old),('timed_new',new)]:
 run(name,[compiler,*common,'-I',build/'include/c++/v1',r/'docs/progress/R111/code/condition_cancel_probe.cpp',out/'identity.o',
 '-nostdlib++','-L',build/'lib','-Wl,-rpath-link,'+str(build/'lib'),'-lc++','-lc++abi','-latomic','-pthread','-ldl','-o',out/name])
 run('bindings_'+name,['readelf','--dyn-syms','--wide',out/name])
run('artifacts',['sha256sum',*sorted(out.iterdir())])
