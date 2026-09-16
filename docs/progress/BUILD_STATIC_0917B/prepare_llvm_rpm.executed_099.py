import hashlib
import json
import os
import shutil
from pathlib import Path

p=Path('progress/BUILD_STATIC_0917B')
t=Path('tmp/STATIC_0917B').resolve()
rpm=t/'rpm-aarch64'
old=Path('tmp/NIGHT_0917/finish_validation/rpm').resolve()
for name in ['SOURCES','SPECS','BUILD','BUILDROOT','RPMS','SRPMS']:
    (rpm/name).mkdir(parents=True,exist_ok=True)
for src in sorted((old/'SOURCES').iterdir()):
    dest=rpm/'SOURCES'/src.name
    assert not dest.exists()
    # Reference the original payload through its read-only container mount.
    dest.symlink_to('/tmp/night/finish_validation/rpm/SOURCES/'+src.name)
for name in ['llvm.spec','libcxx-runtimes.spec']:
    shutil.copy2(t/'specs'/name,rpm/'SPECS'/name)
macro=next(s[1:] for s in Path('codes/qb/TIZEN/Tizen/Tizen-Base-Toolchain/project_config').read_text().splitlines() if s.startswith('%toolchain_is() '))
base=json.loads((p/'private_root_command.json').read_text())
cmd=base+['--','rpmbuild','-bb','--target','aarch64',
    '--define','_topdir /tmp/task/rpm-aarch64','--define','_tmppath /tmp/task/rpm-aarch64',
    '--define','_smp_mflags -j1','--define','_smp_build_ncpus 1','--define','mlgo_build_jobs 1',
    '--define','_host aarch64-tizen-linux-gnu',
    '--define','__cc /usr/bin/aarch64-tizen-linux-gnu-clang',
    '--define','__cxx /usr/bin/aarch64-tizen-linux-gnu-clang++',
    '--define','_toolchain clang','--define',macro,
    '/tmp/task/rpm-aarch64/SPECS/llvm.spec']
(p/'llvm_full_rpmbuild_command.json').write_text(json.dumps(cmd,indent=2))
(p/'rpm_spec_hashes.json').write_text(json.dumps({q.name:hashlib.sha256(q.read_bytes()).hexdigest() for q in (rpm/'SPECS').iterdir()},indent=2))
print('Full rpmbuild -bb command prepared; no prebuilt .a or old GCC tree substituted for full package build.')
