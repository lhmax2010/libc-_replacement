#!/usr/bin/env python3
import pathlib, subprocess, sys
ROOT=pathlib.Path(__file__).resolve().parents[4]
OUT=pathlib.Path(__file__).resolve().parent
OLD=pathlib.Path('/home/toolchain/development/libc++_replacement')
REC=ROOT/'docs/progress/API_0911/record.py'
TMP=ROOT/'tmp/P11_0916/controls'
TMP.mkdir(parents=True,exist_ok=True)
def run(stem,args):
    subprocess.run([sys.executable,str(REC),str(OUT/'raw'/stem),'nice','-n','15','ionice','-c','3',*map(str,args)],check=True)
src=ROOT/'docs/progress/R78/code'
lib=OLD/'tmp/R68/build-patched-x86_64/libcxx/test-suite-install'
wrapper=OLD/'progress/R33/tools/tizen-clang++'
flags=['-std=c++17','-O1','-g','-nostdinc++','-I',lib/'include/c++/v1']
link=['-nostdlib++','-L',lib/'lib','-Wl,--no-as-needed','-lc++','-lc++abi','-Wl,--as-needed','-pthread','-ldl']
run('control_build_gnu_main',['g++','-std=c++17','-O1','-g',src/'new_main.cpp','-pthread','-ldl','-o',TMP/'gnu_main'])
run('control_build_llvm_plugin',[wrapper,*flags,'-fPIC','-shared',src/'old_plugin.cpp',*link,'-o',TMP/'llvm_plugin.so'])
run('control_build_export_main',[wrapper,*flags,src/'new_main.cpp',*link,'-Wl,--export-dynamic','-o',TMP/'export_main'])
run('control_build_maps_arm',[OLD/'progress/R68/tools/armv7l_lit_host_clangxx.sh','-x','c','-shared','-fPIC','-fvisibility=hidden',OUT/'maps.c','-o',TMP/'maps-arm.so'])
print(TMP)
