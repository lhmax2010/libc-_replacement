"""Compare archived evidence, record hashes, check staged scope without editing inputs."""
import hashlib
import json
import re
import subprocess
from pathlib import Path

source = Path('progress/BUILD_W2C_0915/upstream_release')
dest = Path('docs/progress/BUILD_W2C_0915/upstream_release')
records = []
for f in sorted(dest.rglob('*')):
    if not f.is_file() or f.name == 'SHA256SUMS.tsv':
        continue
    rel = f.relative_to(dest)
    body = f.read_bytes()
    assert body == (source / rel).read_bytes(), str(rel)
    assert not re.search(rb'(?:ghp_[A-Za-z0-9]{30,}|github_pat_[A-Za-z0-9_]{30,}|-----BEGIN (?:OPENSSH|RSA) PRIVATE KEY-----)', body), str(rel)
    records.append((str(rel), len(body), hashlib.sha256(body).hexdigest()))
manifest = 'path\tbytes\tsha256\n' + ''.join(f'{r}\t{s}\t{h}\n' for r, s, h in records)
for directory in (source, dest):
    (directory / 'SHA256SUMS.tsv').write_text(manifest)
staged = subprocess.run(['git','diff','--cached','--name-only','-z'], capture_output=True, check=True).stdout
names = [x.decode() for x in staged.split(b'\0') if x]
assert names and all(x.startswith(str(dest) + '/') for x in names)
print(json.dumps(dict(archived_files_checked=len(records), staged_files=len(names),
                     bytes_equal=True, staged_scope_ok=True, token_pattern_scan='PASS',
                     note='Hash manifest excludes itself and later delivery records; public CI secret-variable placeholders are not credentials.'), indent=2))
