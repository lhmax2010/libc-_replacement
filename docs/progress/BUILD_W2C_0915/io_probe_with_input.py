"""Add the currently compiled source to the unchanged read/write probe."""
import hashlib
import subprocess
import sys
import time
from pathlib import Path

source = Path('/home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/BUILD-W2B-llvm-armv7l/local/BUILD-ROOTS/scratch.armv7l.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/llvm/lib/Remarks/BitstreamRemarkParser.cpp')
start = time.monotonic()
print('input_read_start='+str(source), flush=True)
content = source.read_bytes()
print('input_read_seconds=%.6f bytes=%d sha256=%s' % (time.monotonic()-start, len(content), hashlib.sha256(content).hexdigest()), flush=True)
r = subprocess.run([sys.executable, str(Path(__file__).with_name('io_probe.py'))])
print('combined_total_seconds=%.6f' % (time.monotonic()-start), flush=True)
raise SystemExit(r.returncode)
