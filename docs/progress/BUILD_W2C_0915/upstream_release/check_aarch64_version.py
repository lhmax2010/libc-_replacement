"""Run only --version with a private loader prefix; no build-root edits."""
import hashlib
import json
from pathlib import Path
import resource
import shlex
import shutil
import subprocess
import tempfile

out = Path('progress/BUILD_W2C_0915/upstream_release')
root = Path('tmp/GBS-ROOT/BUILD-W2C-bpftrace-aarch64-gcc/local/BUILD-ROOTS/scratch.aarch64.0/usr/lib64')
prefix = Path(tempfile.mkdtemp(prefix='version-sysroot.', dir='tmp/BUILD_W2C_0915')).resolve()
(prefix / 'lib').mkdir()
copied = []
for name in ['ld-linux-aarch64.so.1', 'libc.so.6', 'libm.so.6']:
    source = root / name
    target = prefix / 'lib' / name
    shutil.copy2(source, target)
    src_hash = hashlib.sha256(source.read_bytes()).hexdigest()
    assert hashlib.sha256(target.read_bytes()).hexdigest() == src_hash
    copied.append(dict(source=str(source), target=str(target), sha256=src_hash))
binary = Path('tmp/BUILD_W2C_0915/bpf-prebuilt.vuDDCU/prebuilt-static-bpftrace/aarch64/usr/bin/bpftrace').resolve()
args = ['timeout', '-k', '2s', '30s', 'nice', '-n', '19', 'ionice', '-c', '3',
        '/usr/bin/qemu-arm64-static', '-L', str(prefix), str(binary), '--version']
resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
r = subprocess.run(args, capture_output=True, text=True)
record = dict(prefix=str(prefix), copied=copied, command=shlex.join(args),
              exitcode=r.returncode, stdout=r.stdout, stderr=r.stderr,
              scope='QEMU user-mode --version only; no board or tracing')
(out / 'aarch64_version_private_prefix.json').write_text(json.dumps(record, indent=2)+'\n')
print(json.dumps(record, indent=2))
raise SystemExit(r.returncode)
