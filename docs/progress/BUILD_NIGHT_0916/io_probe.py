"""Small read/write/fsync test in a newly created private temporary directory."""
import hashlib
import json
import os
from pathlib import Path
import tempfile
import time

start = time.monotonic()
directory = Path(tempfile.mkdtemp(prefix='night-io.', dir='tmp'))
path = directory / 'probe.bin'
block = b'BPFTRACE_IO_CHECK\n' * 262144
with path.open('wb') as f:
    f.write(block)
    f.flush()
    os.fsync(f.fileno())
with path.open('rb') as f:
    actual = hashlib.sha256(f.read()).hexdigest()
assert actual == hashlib.sha256(block).hexdigest()
elapsed = time.monotonic() - start
print(json.dumps(dict(path=str(path), bytes=len(block), seconds=elapsed,
                     result='PASS' if elapsed <= 30 else 'PAUSE_REQUIRED')))
# Only our explicit new probe file is removed, no pre-existing input is touched.
path.unlink()
directory.rmdir()
raise SystemExit(0 if elapsed <= 30 else 30)
