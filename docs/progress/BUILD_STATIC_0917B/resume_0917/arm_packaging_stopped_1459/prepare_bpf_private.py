"""Prepare isolated writable RPM input directories without sudo or source edits.

Run under cell_guard. Copy only usr and the RPM database; retain the old build
root read-only. Public loader/RPM configuration is copied by an explicit list.
"""
import hashlib
import json
import shlex
import shutil
import subprocess
import sys
from pathlib import Path

project = Path.cwd()
out = project / 'progress/BUILD_STATIC_0917B/resume_0917'
task = project / 'tmp/STATIC_0917B'
arch = sys.argv[1]
assert arch in ('armv7l', 'aarch64')
source = project / f'tmp/GBS-ROOT/BUILD-W2C-bpftrace-{arch}-libcxx/local/BUILD-ROOTS/scratch.{arch}.0'
dest = task / f'bpf-input-{arch}'
assert not dest.exists()
records = []

def run(cmd, capture=True):
    print('COMMAND ' + shlex.join(cmd), flush=True)
    r = subprocess.run(cmd, capture_output=capture, text=True)
    row = {'command': shlex.join(cmd), 'exitcode': r.returncode}
    if capture:
        row.update(stdout=r.stdout, stderr=r.stderr)
        print(r.stdout, end='', flush=True)
        print(r.stderr, end='', file=sys.stderr, flush=True)
    records.append(row)
    (out / f'bpf-private-{arch}-commands.json').write_text(json.dumps(records, indent=2))
    assert r.returncode == 0, cmd
    return r.stdout if capture else ''

du = run(['du', '-s', '--block-size=1', '--exclude=upgrade', str(source/'usr'), str(source/'var/lib/rpm')])
total = sum(int(line.split()[0]) for line in du.splitlines())
assert total <= 3*1024**3
print(f'COPY_SIZE_GATE {total} <= {3*1024**3}', flush=True)
for name in ('usr', 'rpmdb', 'etc', 'root'):
    (dest/name).mkdir(parents=True, exist_ok=False)
for old, new in [(source/'usr',dest/'usr'),(source/'var/lib/rpm',dest/'rpmdb')]:
    run(['rsync', '-aH', '--no-owner', '--no-group', '--exclude=/share/upgrade',
         '--out-format=COPY_ENTRY %n', str(old)+'/', str(new)+'/'], capture=False)
# Do not copy shadow, credentials, or arbitrary user/home configuration.
public = ['ld.so.conf', 'ld.so.cache', 'ld.so.conf.d', 'passwd', 'group',
          'nsswitch.conf', 'os-release', 'rpm', 'localtime']
copied = []
for name in public:
    src = source/'etc'/name
    if not src.exists():
        copied.append({'name': name, 'status': 'NOT_OBSERVED'})
        continue
    target = dest/'etc'/name
    if src.is_dir():
        shutil.copytree(src,target,symlinks=True)
    else:
        shutil.copy2(src,target,follow_symlinks=True)
    copied.append({'name': name, 'status': 'COPIED_PUBLIC_CONFIGURATION'})
base = ['bwrap', '--unshare-user', '--uid', '0', '--gid', '0',
        '--ro-bind', str(source), '/', '--bind', str(dest/'usr'), '/usr',
        '--bind', str(dest/'rpmdb'), '/var/lib/rpm',
        '--bind', str(dest/'etc'), '/etc', '--bind', str(dest/'root'), '/root',
        '--dev', '/dev', '--proc', '/proc', '--tmpfs', '/tmp',
        '--bind', str(task), '/tmp/task',
        '--ro-bind', str(project/'tmp/NIGHT_0917'), '/tmp/night',
        '--setenv', 'PATH', '/bin:/usr/bin:/sbin:/usr/sbin',
        '--setenv', 'CMAKE_BUILD_PARALLEL_LEVEL', '1',
        '--setenv', 'RPM_BUILD_NCPUS', '1', '--setenv', 'MAKEFLAGS', '-j1',
        '--setenv', 'QEMU_RESERVED_VA', '0x100000000', '--setenv', 'LC_ALL', 'C']
(out/f'bpf-private-{arch}-base.json').write_text(json.dumps(base,indent=2))
run(base+['--','id'])
run(base+['--','rpm','-qa','--qf','%{NAME}\t%{VERSION}-%{RELEASE}\t%{ARCH}\n'])
(out/f'bpf-private-{arch}-result.json').write_text(json.dumps({
    'status': 'PRIVATE_INPUT_ROOT_PREPARED_NOT_INSTALLED', 'source':str(source),
    'destination':str(dest), 'allocated_source_bytes':total, 'public_etc':copied,
    'excluded':'usr/share/upgrade (unreadable; no requested RPM payload there)',
    'sudo':False, 'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest()},indent=2))
