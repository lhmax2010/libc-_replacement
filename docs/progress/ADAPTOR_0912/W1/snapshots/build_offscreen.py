#!/usr/bin/env python3
import pathlib,sys,json,csv,shlex,hashlib
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[1]))
from common import ROOT as r,EXT as e,record
o=r/'docs/progress/ADAPTOR_0912/W1';t=r/'tmp/ADAPTOR_0912/W1';t.mkdir(parents=True,exist_ok=True)
old=r/'tmp/API_0911/W1_CONT';c=e/'tmp/corpus/extracted'
core=c/'dali2-2.5.26-1.src.rpm/unpacked/dali2-2.5.26.tar.gz/dali2-2.5.26'
ad=c/'dali2-adaptor-2.5.26-1.src.rpm/unpacked/dali2-adaptor-2.5.26.tar.gz/dali2-adaptor-2.5.26'
rows=list(csv.DictReader((r/'docs/progress/P7_0909/inputs/binary_package_records.tsv').open(),delimiter='\t'))
x=[x for x in rows if x['arch']=='armv7l' and x['name']=='dali2-adaptor'][0]
rpm=e/'tmp/R11/rpms'/x['repo_id']/x['location'];assert hashlib.sha256(rpm.read_bytes()).hexdigest()==x['checksum']
sdk=t/'adaptor_sdk';sdk.mkdir(exist_ok=True)
rc,_=record('W1','013_extract_arm_adaptor',['bash','-o','pipefail','-c','cd '+shlex.quote(str(sdk))+' && rpm2cpio '+shlex.quote(str(rpm))+' | cpio -idmu --quiet --no-absolute-filenames']);assert rc==0
gnu=old/'arm/sdk/libstdc++-devel/usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/c++'
libs=list(sdk.rglob('libdali2-adaptor.so.2.0.0'));assert len(libs)==1
cmd=[e/'progress/R33/tools/tizen-clang++','--target=armv7l-tizen-linux-gnueabi','--sysroot='+str(e/'tmp/GBS-ROOT/R30-semantic-libcxx-armv7l-20260812-01/local/BUILD-ROOTS/scratch.armv7l.0'),'--ld-path='+str(e/'progress/R36/tools/armv7l-ld'),'-march=armv7-a','-mfpu=neon','-mfloat-abi=softfp','-mthumb','-std=c++17','-O0','-g','-nostdinc++','-I'+str(gnu),'-I'+str(gnu/'armv7l-tizen-linux-gnueabi'),'-nostdlib++','-I'+str(core),'-I'+str(ad),'-include',r/'docs/progress/API_0911/W1_CONT/arm_monitor.h',o/'offscreen_probe.cpp',libs[0],'-L'+str(old/'arm/dali_core_cxx'),'-Wl,--allow-shlib-undefined','-ldali2-core','-L'+str(r/'tmp/IMPL_0908/build-armv7l/lib'),'-Wl,--no-as-needed','-lc++','-lc++abi',old/'arm/sdk/libstdc++/usr/lib/libstdc++.so.6','-ldl','-o',t/'offscreen_probe']
# BaseHandle native ABI methods are resolved by the actual GNU board core at runtime;
# inspect DT_NEEDED and record use of the temporary link input, do not claim a mixed core run.
rc,_=record('W1','014_build_offscreen',cmd)
if rc==0:record('W1','015_offscreen_needed',['readelf','-d',t/'offscreen_probe'])
sys.exit(rc)
