#!/usr/bin/env python3
"""只复制本次证据所需的文本快照；不归档任何构建产物。"""
import pathlib, shutil, hashlib, csv
r=pathlib.Path('docs/progress/R116'); d=r/'snapshots'; d.mkdir(exist_ok=True)
items={
 'record.py':'docs/progress/R114/code/record.py',
 'condition_cancel_probe.cpp':'docs/progress/R111/code/condition_cancel_probe.cpp',
 'shared_probe.cpp':'tmp/R115-W4/probe.cpp',
 'trace.h':'tmp/R115-W4/trace.h',
 'condition_variable_removed.cpp':'tmp/R115-W4/condition_variable_removed.cpp',
 'shared_build.py':'tmp/R115-W4/build.py',
 'install_manifest_llvm-libgcc.txt':'tmp/R116/build-native/install_manifest_llvm-libgcc.txt',
 'CMakeCache.txt':'tmp/R116/build-native/CMakeCache.txt',
 'generated-gcc_s.ver':'tmp/R116/build-native/gcc_s.ver',
 'llvm-libgcc-CMakeLists.txt':'codes/llvm/llvm-libgcc/CMakeLists.txt',
 'gcc_s.ver.in':'codes/llvm/llvm-libgcc/gcc_s.ver.in',
 'generate_version_script.py':'codes/llvm/llvm-libgcc/generate_version_script.py',
 'UnwindLevel1.c':'codes/llvm/libunwind/src/UnwindLevel1.c',
 'Unwind-EHABI.cpp':'codes/llvm/libunwind/src/Unwind-EHABI.cpp',
 'gcc-unwind.inc':'codes/gcc/libgcc/unwind.inc',
 'glibc-unwind-link.c':'/home/toolchain/development/libc++_replacement/tmp/R72/glibc-2.40/misc/unwind-link.c',
 'glibc-unwind.c':'/home/toolchain/development/libc++_replacement/tmp/R72/glibc-2.40/nptl/unwind.c',
}
for p in pathlib.Path('codes/llvm/llvm-libgcc/docs').glob('LLVM*'):items[p.name]=str(p)
base=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/corpus/extracted/lightweight-web-engine-1.4.0-1.src.rpm/unpacked/lightweight-web-engine-1.4.0.tar.gz/lightweight-web-engine-1.4.0')
for p in base.rglob('WorkerThread.cpp'):items['Starfish-WorkerThread.cpp']=str(p)
for p in base.glob('README*'):items['Starfish-'+p.name]=str(p)
for variant in ['control','rollback']:
 for p in pathlib.Path('tmp/R115-W4',variant).rglob('*'):
  if p.is_file() and p.suffix not in ['.so','.o','.a']:
   items['shared_'+variant+'/'+str(p.relative_to(pathlib.Path('tmp/R115-W4',variant)))]=str(p)
with (r/'snapshot_origins.tsv').open('w') as f:
 w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['snapshot','source','sha256'])
 for name,src in items.items():
  target=d/name;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(src,target)
  w.writerow([str(target),src,hashlib.sha256(target.read_bytes()).hexdigest()]);print('SNAPSHOT',src,'->',target)
