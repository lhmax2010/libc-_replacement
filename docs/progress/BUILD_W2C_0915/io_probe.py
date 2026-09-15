import hashlib
import os
import tempfile
import time
from pathlib import Path

root = Path('/home/toolchain/development/libc++_replacement/tmp/GBS-ROOT')
payload = b'W2B_IO_PROBE_0915' * (1024 * 1024 // 16)
start = time.monotonic()
with tempfile.TemporaryDirectory(prefix='w2c-io-probe-', dir=root) as folder:
    path = Path(folder) / 'probe.bin'
    t = time.monotonic()
    with path.open('wb') as stream:
        for _ in range(8):
            stream.write(payload)
        stream.flush()
        os.fsync(stream.fileno())
    print(f'write_fsync_seconds={time.monotonic()-t:.6f}', flush=True)
    t = time.monotonic()
    content = path.read_bytes()
    assert content == payload * 8
    print(f'read_verify_seconds={time.monotonic()-t:.6f}', flush=True)
    print(f'bytes={len(content)} sha256={hashlib.sha256(content).hexdigest()}', flush=True)
print(f'total_seconds={time.monotonic()-start:.6f}')
print('probe_file_and_directory_removed=yes')
