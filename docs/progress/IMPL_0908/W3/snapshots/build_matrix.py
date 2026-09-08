#!/usr/bin/env python3
"""两套真实库；逐个分开编译/链接，避免 ARM 包装器合并启动问题。"""
import pathlib,subprocess,sys
r=pathlib.Path.cwd();arch=sys.argv[1];out=r/'tmp/IMPL_0908'/('probes-'+arch);out.mkdir(exist_ok=True)
suffix='native' if arch=='x86_64' else 'armv7l'
old=r/'tmp/IMPL_0908'/('build-baseline-'+suffix);new=r/'tmp/IMPL_0908'/('build-'+suffix)
cc='/home/toolchain/development/libc++_replacement/progress/'+('R33/tools/tizen-clang++' if arch=='x86_64' else 'R36/tools/armv7l-clang++')
attempt=sys.argv[2] if len(sys.argv)>2 else ''
raw=r/'docs/progress/IMPL_0908/W3/raw'/('build_probes_'+arch+('_'+attempt if attempt else ''));n=0
def run(label,args):
 global n;n+=1
 subprocess.run([sys.executable,'docs/progress/R114/code/record.py',str(raw/f'{n:03}_{label}'),'nice','-n','15','ionice','-c','3',*map(str,args)],check=True)
def compile(name,src,headers,defs=(),opt='-O0'):
 obj=out/(name+'.o')
 run('compile_'+name,[cc,'-std=c++20',opt,'-g','-fno-inline','-fPIC','-fno-access-control','-nostdinc++','-I',headers/'include/c++/v1',*defs,'-c',src,'-o',obj]);return obj
def link(name,objs,build):
 run('link_'+name,[cc,*objs,'-nostdlib++','-L',build/'lib','-Wl,-rpath-link,'+str(build/'lib'),'-lc++','-lc++abi','-latomic','-pthread','-ldl','-o',out/name])
 run('versions_'+name,['readelf','--dyn-syms','--wide',out/name])
identity=compile('identity',r/'docs/progress/IMPL_0908/code/runtime_identity.cpp',new)
for variant,build in [('old',old),('new',new)]:
 for name,src,defs,opt in [
  ('normal',r/'docs/progress/IMPL_0908/code/normal_probe.cpp',['-DEXPECT_NEW='+str(int(variant=='new'))],'-O2'),
  ('shared',r/'docs/progress/IMPL_0908/code/shared_probe.cpp',[],'-O2'),
  ('buffer_cancel',r/'docs/progress/IMPL_0908/code/buffer_cancel.cpp',[],'-O0')]:
  obj=compile(variant+'_'+name,src,build,defs,opt);link(variant+'_'+name,[obj,identity],build)
for name,src in [('wait','R111/code/condition_cancel_probe.cpp'),('facility','R113/code/item1_facility_cancel.cpp')]:
 obj=compile(name,r/'docs/progress'/src,new);link(name,[obj,identity],new)
objects={}
for variant,build in [('old',old),('new',new)]:
 objects[variant]=compile('quiet_'+variant,r/'docs/progress/R112/code/item1_oldcaller_cancel.cpp',build,['-DWORKER_NO_PUTS','-DEXPECT_CLEAN='+str(int(variant=='new'))])
for name,obj,lib in [('old_elf','old',old),('old_relinked','old',new),('new_object','new',new),('new_prelinked_old','new',old)]:
 link(name,[objects[obj],identity],lib)
run('sha256',['sha256sum',*sorted(out.iterdir())])
