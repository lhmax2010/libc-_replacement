"""Inspect completed LLVM RPMs and the archive extracted from the actual RPM."""
import hashlib
import json
import shlex
import subprocess
import sys
from pathlib import Path

arch = sys.argv[1]
assert arch in ('aarch64', 'armv7l')
base = Path.cwd()
out = base / 'progress/BUILD_WEEKEND_0918/llvm_retry_partial' / ('verify_' + arch)
out.mkdir(exist_ok=True)
task = base / 'tmp/WEEKEND_0918'
top = task / ('rpm-aarch64' if arch == 'aarch64' else 'rpm-arm-llvm')
extract = task / ('verified-rpm-payload-' + arch)
extract.mkdir(exist_ok=False)
history = (out / 'commands.jsonl').open('x')
def command(args, name, cwd=None):
    r = subprocess.run(args, cwd=cwd, capture_output=True)
    (out / (name + '.stdout')).write_bytes(r.stdout)
    (out / (name + '.stderr')).write_bytes(r.stderr)
    history.write(json.dumps({'command': shlex.join(args), 'cwd': str(cwd or base),
                              'exitcode': r.returncode}) + '\n')
    history.flush()
    if r.returncode:
        raise RuntimeError(name + ' exit ' + str(r.returncode))
    return r.stdout.decode(errors='replace')
def sha(path):
    with path.open('rb') as f:
        return hashlib.file_digest(f, 'sha256').hexdigest()
rpms = sorted((top / 'RPMS').rglob('*.rpm'))
assert rpms
rows = []
byname = {}
for i, rpm in enumerate(rpms):
    command(['rpm', '--checksig', '--nosignature', str(rpm)], f'{i:02d}_digests')
    fields = command(['rpm', '-qp', '--qf', '%{NAME}\t%{VERSION}\t%{RELEASE}\t%{ARCH}\n', str(rpm)], f'{i:02d}_identity').strip().split('\t')
    assert fields[3] == arch, fields
    files = command(['rpm', '-qpl', str(rpm)], f'{i:02d}_files').splitlines()
    row = {'path': str(rpm), 'name': fields[0], 'version': fields[1],
           'release': fields[2], 'arch': fields[3], 'bytes': rpm.stat().st_size,
           'sha256': sha(rpm), 'files': files}
    rows.append(row)
    assert fields[0] not in byname
    byname[fields[0]] = row
libdir = '/usr/lib64' if arch == 'aarch64' else '/usr/lib'
static = byname['llvm-static-devel']
assert libdir + '/libclang.a' in static['files']
shared = byname['libllvm']
needed_candidates = [s for s in shared['files'] if '/libLLVM.so.' in s]
assert needed_candidates
for label, row, patterns in [
        ('static', static, ['.' + libdir + '/libclang.a']),
        ('shared', shared, ['.' + libdir + '/libLLVM.so*'])]:
    script = 'rpm2cpio ' + shlex.quote(row['path']) + ' | cpio -idmu --quiet ' + ' '.join(shlex.quote(p) for p in patterns)
    command(['bash', '-o', 'pipefail', '-c', script], label + '_extract', extract)
archive = extract / libdir.lstrip('/') / 'libclang.a'
strings = command(['strings', str(archive)], 'libclang_strings')
one = sum('_ZNSt3__1' in s for s in strings.splitlines())
cxx11 = sum('_ZNSt7__cxx11' in s for s in strings.splitlines())
assert one > 0 and cxx11 == 0, (one, cxx11)
command(['ar', 't', str(archive)], 'libclang_members')
command(['readelf', '-h', str(archive)], 'libclang_elf_headers')
dso = next((extract / s.lstrip('/') for s in needed_candidates
            if (extract / s.lstrip('/')).is_file()), None)
assert dso
dynamic = command(['readelf', '-d', str(dso)], 'libllvm_dynamic')
assert 'libc++.so.1' in dynamic and 'libstdc++.so' not in dynamic
result = {'status': 'RPM_PAYLOAD_CHECK_PASS', 'arch': arch, 'rpms': rows,
          'libclang_archive_sha256': sha(archive),
          'libclang_strings_std_namespace_lines': one,
          'libclang_strings_cxx11_namespace_lines': cxx11,
          'libllvm_checked_path': str(dso),
          'limitation': 'Symbol markers establish observed archive ABI traces, not complete functional equivalence.'}
(out / 'result.json').write_text(json.dumps(result, indent=2))
print(json.dumps({k: v for k, v in result.items() if k != 'rpms'}, indent=2))
print('RPM_COUNT', len(rpms))
