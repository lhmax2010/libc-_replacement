#!/usr/bin/env python3
"""为两架构补建记录 LLVM_22 定时入口的旧 ELF；运行时将装载新库。"""
import pathlib,subprocess,sys
r=pathlib.Path.cwd();n=0
for arch,suffix,tool in [('x86_64','native','R33/tools/tizen-clang++'),('armv7l','armv7l','R36/tools/armv7l-clang++')]:
 cc='/home/toolchain/development/libc++_replacement/progress/'+tool
 old=r/'tmp/IMPL_0908'/('build-baseline-'+suffix);out=r/'tmp/IMPL_0908'/('probes-'+arch)
 commands=[['compile',cc,'-std=c++20','-O0','-g','-fno-inline','-fPIC','-nostdinc++','-I',old/'include/c++/v1','-c',r/'docs/progress/R111/code/condition_cancel_probe.cpp','-o',out/'old_timed.o'],
 ['link',cc,out/'old_timed.o',out/'identity.o','-nostdlib++','-L',old/'lib','-Wl,-rpath-link,'+str(old/'lib'),'-lc++','-lc++abi','-latomic','-pthread','-ldl','-o',out/'old_timed'],
 ['symbols','readelf','--dyn-syms','--wide',out/'old_timed'],['hash','sha256sum',out/'old_timed']]
 for label,*cmd in commands:
  n+=1;subprocess.run([sys.executable,'docs/progress/R114/code/record.py',f'docs/progress/IMPL_0908/W3/raw/build_old_timed/{n:02}_{arch}_{label}','nice','-n','15','ionice','-c','3',*map(str,cmd)],check=True)
