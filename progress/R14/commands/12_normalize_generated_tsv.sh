#!/usr/bin/env bash
set -euo pipefail
set -x

workspace=/home/toolchain/development/libc++_replacement
cd "$workspace"

python3 - <<'PY'
from pathlib import Path

paths = sorted(Path('progress/R14/tables').glob('*.tsv'))
total_before = 0
for path in paths:
    data = path.read_bytes()
    before = data.count(b'\r\n')
    total_before += before
    normalized = data.replace(b'\r\n', b'\n')
    path.write_bytes(normalized)
    after = normalized.count(b'\r\n')
    print(f'{path}\tCRLF_BEFORE={before}\tCRLF_AFTER={after}')
    assert after == 0
print(f'FILES={len(paths)}')
print(f'TOTAL_CRLF_BEFORE={total_before}')
print('NORMALIZATION=LF_ONLY')
PY
