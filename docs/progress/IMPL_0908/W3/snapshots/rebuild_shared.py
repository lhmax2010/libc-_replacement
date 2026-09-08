#!/usr/bin/env python3
"""只重编观测器修正后的探针；不改运行库。"""
import pathlib,subprocess,sys
r=pathlib.Path.cwd();n=0
for arch,suffix,cc in [('x86_64','native','R33/tools/tizen-clang++'),('armv7l','armv7l','R36/tools/armv7l-clang++')]:
 out=r/'tmp/IMPL_0908'/('probes-'+arch);cc='/home/toolchain/development/libc++_replacement/progress/'+cc
 for variant in ['old','new']:
  build=r/'tmp/IMPL_0908'/('build-'+('baseline-' if variant=='old' else '')+suffix);obj=out/(variant+'_shared.o')
  commands=[['compile',cc,'-std=c++20','-O2','-g','-fno-inline','-fPIC','-fno-access-control','-nostdinc++','-I',build/'include/c++/v1','-c',r/'docs/progress/IMPL_0908/code/shared_probe.cpp','-o',obj],
   ['link',cc,obj,out/'identity.o','-nostdlib++','-L',build/'lib','-Wl,-rpath-link,'+str(build/'lib'),'-lc++','-lc++abi','-latomic','-pthread','-ldl','-o',out/(variant+'_shared')]]
  for label,*cmd in commands:
   n+=1;subprocess.run([sys.executable,'docs/progress/R114/code/record.py',f'docs/progress/IMPL_0908/W3/raw/observer_build/{n:02}_{arch}_{variant}_{label}','nice','-n','15','ionice','-c','3',*map(str,cmd)],check=True)
