import csv
import hashlib
from pathlib import Path

p = Path('progress/BUILD_W2C_0915')
with (p/'PACKAGE_STATUS.tsv').open() as f:
    rows = list(csv.DictReader(f, delimiter='\t'))
assert len(rows) == 11
assert sum(r['current_status'] == '已完成并推送' for r in rows) == 9
assert len({r['package'] for r in rows}) == 11
expected = {
    'govern_build_v1.py': '475a3862926d855c1fd1be4ad74971a336741d4076e7048393b8812fa1073787',
    'llvm_resume_executed_retry.sh': 'cf6d51bf60c25ef1587462d31b1c8b255f393706147a9769465eae23468d3c56',
}
for name, digest in expected.items():
    assert hashlib.sha256((p/name).read_bytes()).hexdigest() == digest
for command in (p/'raw').glob('*.command.txt'):
    prefix = command.name.removesuffix('.command.txt')
    if prefix.startswith('036_'):
        continue
    for suffix in ('stdout.txt', 'stderr.txt', 'exitcode'):
        assert (p/'raw'/f'{prefix}.{suffix}').exists(), (prefix, suffix)
    int((p/'raw'/f'{prefix}.exitcode').read_text().strip())
report = (p/'REPORT.md').read_text()
assert 'PARTIAL' in report and '没有新增通过格' in report
assert '未完整另存' in report
print('PASS: 11 packages, 9 previously completed; no new PASS; executed retry script hashes exact; finished logged commands have exit codes.')
