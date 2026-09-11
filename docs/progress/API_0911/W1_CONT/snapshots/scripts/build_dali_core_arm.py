#!/usr/bin/env python3
import pathlib,subprocess,sys,os
r=pathlib.Path.cwd();o=r/'docs/progress/API_0911/W1_CONT';t=r/'tmp/API_0911/W1_CONT/arm/dali_core_cxx';b=r/'tmp/IMPL_0908/build-armv7l';base=pathlib.Path('/home/toolchain/development/libc++_replacement')
src=base/'tmp/corpus/extracted/dali2-2.5.26-1.src.rpm/unpacked/dali2-2.5.26.tar.gz/dali2-2.5.26/build/tizen'
flags='--target=armv7l-tizen-linux-gnueabi --sysroot='+str(base/'tmp/GBS-ROOT/R30-semantic-libcxx-armv7l-20260812-01/local/BUILD-ROOTS/scratch.armv7l.0')+' --ld-path='+str(base/'progress/R36/tools/armv7l-ld')+' -march=armv7-a -mfpu=neon -mfloat-abi=softfp -mthumb -nostdinc++ -I'+str(b/'include/c++/v1')
link='-nostdlib++ -L'+str(b/'lib')+' -lc++ -lc++abi'
def run(stem,cmd):
 rc=subprocess.run([sys.executable,str(r/'docs/progress/API_0911/record.py'),str(o/'raw'/(stem+os.environ.get('API_CORE_ARM_ATTEMPT',''))),'nice','-n','15','ionice','-c','3','env','PYTHONDONTWRITEBYTECODE=1',*cmd]).returncode
 if rc:raise SystemExit(rc)
run('dali_arm_core_configure',['cmake','-S',str(src),'-B',str(t),'-DCMAKE_SYSTEM_NAME=Linux','-DCMAKE_SYSTEM_PROCESSOR=armv7l','-DCMAKE_BUILD_TYPE=Release','-DCMAKE_C_COMPILER='+str(base/'progress/R33/tools/tizen-clang'),'-DCMAKE_C_FLAGS='+flags,'-DCMAKE_CXX_COMPILER='+str(base/'progress/R33/tools/tizen-clang++'),'-DCMAKE_CXX_FLAGS='+flags,'-DCMAKE_SHARED_LINKER_FLAGS='+link,'-DCMAKE_EXE_LINKER_FLAGS='+link,'-DENABLE_PKG_CONFIGURE=OFF','-DCMAKE_EXPORT_COMPILE_COMMANDS=ON'])
run('dali_arm_core_build',['cmake','--build',str(t),'--parallel','2','--target','dali2-core'])
