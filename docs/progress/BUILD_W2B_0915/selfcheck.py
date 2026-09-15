import ast
import csv
import hashlib
from pathlib import Path

root = Path('/home/toolchain/development/libc++_replacement/progress/BUILD_W2B_0915')
for name, expected in [('MATRIX.tsv', 6), ('PACKAGE_STATUS.tsv', 11)]:
    with (root / name).open() as stream:
        reader = csv.reader(stream, delimiter='\t')
        rows = list(reader)
    assert len(rows) == expected + 1, (name, len(rows))
    assert all(len(r) == len(rows[0]) for r in rows), name
    print(f'{name}: data_rows={expected}, columns={len(rows[0])}, PASS')
for path in root.glob('*.py'):
    ast.parse(path.read_text(), filename=str(path))
for path in sorted((root / 'raw').glob('*.command.txt')):
    if path.name.startswith('021_'):
        continue
    prefix = path.name.removesuffix('.command.txt')
    for suffix in ('exitcode', 'stdout.txt', 'stderr.txt', 'started.txt', 'finished.txt'):
        assert (root / 'raw' / f'{prefix}.{suffix}').exists(), (prefix, suffix)
    assert (root / 'raw' / f'{prefix}.exitcode').read_text().strip().isdigit()
print('finished command records complete=PASS; active selfcheck excluded')
with (root / 'spec_audit/inputs.tsv').open() as stream:
    inputs = list(csv.DictReader(stream, delimiter='\t'))
assert len({r['package'] for r in inputs}) == 11
for row in inputs:
    path = root / 'spec_audit' / (row['package'] + '__' + Path(row['spec']).name)
    assert hashlib.sha256(path.read_bytes()).hexdigest() == row['sha256']
print('11 package spec input hashes=PASS')
for path in sorted(root.iterdir()):
    if path.suffix in ('.sh', '.py', '.conf'):
        print(hashlib.sha256(path.read_bytes()).hexdigest(), path.name)
