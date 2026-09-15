import csv
import gzip
import hashlib
import subprocess
from pathlib import Path

root = Path('docs/progress/BUILD_NIGHT_0917')
rows = list(csv.DictReader((root / 'SHA256SUMS.tsv').open(), delimiter='\t'))
for row in rows:
    data = (root / row['path']).read_bytes()
    assert len(data) == int(row['bytes']), row['path']
    assert hashlib.sha256(data).hexdigest() == row['sha256'], row['path']
compressed = list(csv.DictReader((root / 'COMPRESSED_OUTPUTS.tsv').open(), delimiter='\t'))
for row in compressed:
    data = gzip.decompress((root / row['archived_path']).read_bytes())
    assert len(data) == int(row['source_bytes']), row['source_path']
    assert hashlib.sha256(data).hexdigest() == row['source_sha256'], row['source_path']
branch = 'refs/heads/codex/r5-ehabi-diagnosis'
head = subprocess.check_output(['git', 'rev-parse', 'HEAD'], text=True).strip()
remote = subprocess.check_output(['git', 'ls-remote', '--heads', 'origin', branch], text=True).strip()
assert remote == head + '\t' + branch, remote
print('Archive entries verified:', len(rows))
print('Compressed outputs verified:', len(compressed))
print('Local and remote HEAD:', head)
