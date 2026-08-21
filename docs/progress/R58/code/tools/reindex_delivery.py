#!/usr/bin/env python3
from pathlib import Path
import hashlib

root = Path('/home/toolchain/development/libc++_replacement/docs/progress/R58')
for base in (root / 'review_package', root):
    rows = []
    for path in sorted(base.rglob('*')):
        if path.is_file() and path.name != 'INDEX.tsv':
            rel = path.relative_to(base)
            rows.append((str(rel), hashlib.sha256(path.read_bytes()).hexdigest(), str(path.stat().st_size)))
    (base / 'INDEX.tsv').write_text('path\tsha256\tsize_bytes\n' + ''.join('\t'.join(row) + '\n' for row in rows))
