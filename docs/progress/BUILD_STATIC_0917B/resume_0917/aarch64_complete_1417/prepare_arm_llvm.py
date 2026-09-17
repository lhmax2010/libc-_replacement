"""Prepare, but do not launch, incremental armv7l full RPM build."""
import hashlib
import json
from pathlib import Path

project = Path.cwd()
out = project / 'progress/BUILD_STATIC_0917B/resume_0917'
task = project / 'tmp/STATIC_0917B'
root = project / 'tmp/GBS-ROOT/BUILD-W2B-llvm-armv7l/local/BUILD-ROOTS/scratch.armv7l.0'
oldtop = root / 'home/abuild/rpmbuild'
newtop = task / 'rpm-arm-llvm'
comparison = json.loads((out / 'source_payload_comparison.json').read_text())
assert not any(comparison[key] for key in ('only_old', 'only_current', 'different'))
cache = (oldtop / 'BUILD/llvm-22.1.8/build/CMakeCache.txt').read_text()
flags = next(line.split('=', 1)[1] for line in cache.splitlines()
             if line.startswith('CMAKE_CXX_FLAGS:STRING='))
observed = (out / 'raw/101_arm_complete_macro_inputs.stdout.txt').read_text().strip().split('|')
assert observed[0] == 'clang' and observed[1] == flags
assert observed[-3:] == ['-j1', '1', '1']
assert 'CMAKE_CXX_COMPILER:UNINITIALIZED=armv7l-tizen-linux-gnueabi-clang++' in cache
for name in ('RPMS', 'SRPMS', 'BUILDROOT', 'TMP'):
    (newtop / name).mkdir(parents=True, exist_ok=True)
macrofiles = '/usr/lib/rpm/macros:/usr/lib/rpm/macros.d/macros.*:/usr/lib/rpm/platform/%{_target}/macros:/usr/lib/rpm/fileattrs/*.attr:/usr/lib/rpm/tizen/macros:/etc/rpm/macros.*:/etc/rpm/macros:/etc/rpm/%{_target}/macros:/home/abuild/.rpmmacros'
cmd = ['bwrap', '--unshare-user', '--uid', '1000', '--gid', '1000',
       '--ro-bind', str(root), '/', '--dev', '/dev', '--proc', '/proc', '--tmpfs', '/tmp',
       '--bind', str(oldtop / 'BUILD'), '/home/abuild/rpmbuild/BUILD',
       '--bind', str(task), '/tmp/task',
       '--ro-bind', str(root / 'usr/bin/ninja'), '/usr/bin/ninja.real',
       '--ro-bind', str(task / 'ninja-serial'), '/usr/bin/ninja',
       '--setenv', 'PATH', '/bin:/usr/bin:/sbin:/usr/sbin',
       '--setenv', 'QEMU_RESERVED_VA', '0x100000000',
       '--setenv', 'CMAKE_BUILD_PARALLEL_LEVEL', '1',
       '--setenv', 'RPM_BUILD_NCPUS', '1', '--setenv', 'MAKEFLAGS', '-j1',
       '--setenv', 'LC_ALL', 'C', '--chdir', '/home/abuild/rpmbuild', '--',
       'rpmbuild', '--rcfile', '/usr/lib/rpm/rpmrc:/home/abuild/.rpmrc',
       '--macros', macrofiles, '--target', 'armv7l-tizen-linux',
       '--define', '_topdir /home/abuild/rpmbuild',
       '--define', '_rpmdir /tmp/task/rpm-arm-llvm/RPMS',
       '--define', '_srcrpmdir /tmp/task/rpm-arm-llvm/SRPMS',
       '--define', '_buildrootdir /tmp/task/rpm-arm-llvm/BUILDROOT',
       '--define', '_tmppath /tmp/task/rpm-arm-llvm/TMP',
       '--define', '_smp_mflags -j1', '--define', '_smp_build_ncpus 1',
       '--define', 'jobs 1', '--define', 'mlgo_build_jobs 1',
       '--define', '_srcdefattr (-,root,root)', '--define', '_build_create_debug 1',
       '--nosignature', '--noprep', '--noclean', '-bb', '/tmp/task/specs/llvm.spec']
(out / 'arm_llvm_command.json').write_text(json.dumps(cmd, indent=2))
record = {'status': 'PREPARED_NOT_STARTED', 'rpm_outputs': str(newtop),
          'reused_build_tree': str(oldtop / 'BUILD'), 'flags_match_cache': True,
          'sudo_required': False, 'source_payload_comparison': 'source_payload_comparison.json',
          'spec_sha256': hashlib.sha256((task / 'specs/llvm.spec').read_bytes()).hexdigest(),
          'flags': flags, 'macrofiles': macrofiles,
          'rcfiles': '/usr/lib/rpm/rpmrc:/home/abuild/.rpmrc'}
(out / 'arm_llvm_preparation.json').write_text(json.dumps(record, indent=2))
print(json.dumps(record, indent=2))
