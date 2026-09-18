#!/usr/bin/env python3
"""只读磁盘空间与小样本 I/O 探测；不刷新缓存，不作磁盘性能结论。"""
import pathlib
import shutil
import time

root = pathlib.Path.cwd()
free = shutil.disk_usage(root).free
print('disk_free_bytes=' + str(free), flush=True)
if free < 20 * 1024**3:
    raise SystemExit(20)
sample = root / 'docs/progress/R119_batch1/W1/entity_full/std_string.json.gz'
start = time.monotonic()
with sample.open('rb') as f:
    data = f.read(4 * 1024**2)
elapsed = time.monotonic() - start
print(f'io_probe_file={sample}\nbytes_read={len(data)}\nelapsed_seconds={elapsed:.6f}')
print('scope=只读单文件（可能命中缓存），不证明整个磁盘无退化')
raise SystemExit(21 if elapsed > 30 else 0)
