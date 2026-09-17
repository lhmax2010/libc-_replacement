"""Prepare a full armv7l runtime RPM build using the unchanged temporary spec."""
import hashlib
import json
from pathlib import Path

p = Path.cwd()
out = p / 'progress/BUILD_STATIC_0917B/resume_0917'
task = p / 'tmp/STATIC_0917B'
root = p / 'tmp/GBS-ROOT/BUILD-W2B-llvm-armv7l/local/BUILD-ROOTS/scratch.armv7l.0'
sources = p / 'tmp/NIGHT_0917/finish_validation/rpm/SOURCES'
top = task / 'rpm-arm-runtime'
for name in ('BUILD', 'BUILDROOT', 'RPMS', 'SRPMS', 'TMP'):
    (top / name).mkdir(parents=True, exist_ok=True)
cmd = json.loads((out / 'arm_llvm_command.json').read_text())
cmd = cmd[:cmd.index('--')]
# The runtime build has its own clean BUILD directory; do not touch the LLVM tree.
i = cmd.index('--bind')
assert cmd[i+2] == '/home/abuild/rpmbuild/BUILD'
del cmd[i:i+3]
cmd += ['--ro-bind', str(sources), '/tmp/runtime-sources', '--',
        'rpmbuild', '--rcfile', '/usr/lib/rpm/rpmrc:/home/abuild/.rpmrc',
        '--macros', '/usr/lib/rpm/macros:/usr/lib/rpm/macros.d/macros.*:/usr/lib/rpm/platform/%{_target}/macros:/usr/lib/rpm/fileattrs/*.attr:/usr/lib/rpm/tizen/macros:/etc/rpm/macros.*:/etc/rpm/macros:/etc/rpm/%{_target}/macros:/home/abuild/.rpmmacros',
        '--target', 'armv7l-tizen-linux',
        '--define', '_topdir /tmp/task/rpm-arm-runtime',
        '--define', '_sourcedir /tmp/runtime-sources',
        '--define', '_tmppath /tmp/task/rpm-arm-runtime/TMP',
        '--define', '_smp_mflags -j1', '--define', '_smp_build_ncpus 1',
        '--define', 'jobs 1', '--define', '_srcdefattr (-,root,root)',
        '--noclean', '-bb', '/tmp/task/specs/libcxx-runtimes.spec']
(out / 'cell_commands').mkdir(exist_ok=True)
(out / 'cell_commands/runtime-arm-libcxx.json').write_text(json.dumps(cmd, indent=2))
names = ['llvm-22.1.8.tar.gz', 'libcxx-llvm22.map', 'libcxx-runtimes.manifest', 'SOURCE_PROVENANCE']
rows = []
for name in names:
    f = sources / name
    with f.open('rb') as stream:
        sha = hashlib.file_digest(stream, 'sha256').hexdigest()
    rows.append({'path': str(f), 'sha256': sha, 'bytes': f.stat().st_size})
(out / 'runtime_arm_inputs.json').write_text(json.dumps(rows, indent=2))
print('Prepared full -bb; original source, existing Clang, no short-circuit or source edits.')
