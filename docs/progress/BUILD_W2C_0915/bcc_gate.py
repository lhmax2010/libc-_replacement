"""Close six build cells, without treating five repeated checks as five builds."""
import csv
import hashlib
import json
from pathlib import Path

p = Path('progress/BUILD_W2C_0915')
spec = Path('codes/R104/bcc-tools/packaging/bcc-tools.spec')
digest = hashlib.sha256(spec.read_bytes()).hexdigest()
assert digest == '42ea7cc9ca6133c3f71594334718636b5cd6aaf68328c18dcd9016fcb1eed7bc'
prior = list(csv.DictReader(Path('docs/progress/BUILD_W2B_0915/spec_audit/inputs.tsv').open(), delimiter='\t'))
assert next(row for row in prior if row['package']=='bcc-tools')['sha256'] == digest

def check(path, arch, mode):
    text = path.read_text()
    assert text.splitlines()[-1] == 'PASS', path
    data = dict(line.split('=', 1) for line in text.splitlines() if '=' in line)
    assert data['arch'] == arch and data['mode'] == mode
    assert int(data['rpm_count']) == 3
    assert int(data['libcxx_needed']) == (2 if mode == 'libcxx' else 0)
    assert int(data['libstdcxx_needed']) == (2 if mode == 'gcc' else 0)
    return data

matrix = []
for arch in ['aarch64','x86_64','armv7l']:
    for mode in ['libcxx','gcc']:
        if arch != 'armv7l':
            paths = [Path(f'docs/progress/BUILD_W2_0912/checks/bcc_{arch}_{mode}/summary.txt')]
        else:
            paths = [p/f'checks/bcc_{arch}_{mode}/check_{n}/summary.txt' for n in range(1,6)]
        checked = [check(path, arch, mode) for path in paths]
        assert all(data == checked[0] for data in checked)
        matrix.append(dict(arch=arch, mode=mode, result='PASS',
            reused=arch!='armv7l', repeated_checks=len(paths), evidence=list(map(str,paths))))
for name in ['093_bcc_armv7l_libcxx','102_bcc_armv7l_gcc','101_bcc_actual_llvm_input']:
    assert (p/f'raw/{name}.exitcode').read_text().strip() == '0'
for n in range(1,6):
    assert not (p/f'checks/bcc_armv7l_gcc/check_{n}/forbidden_libcxx_lines.txt').read_text()
before = dict(line.split()[::-1] for line in (p/'raw/110_bcc_remote_branches_before.stdout.txt').read_text().splitlines())
assert before['refs/heads/sandbox/lhmax2025/libcxx-migration'] == 'dd26ba03fc8109396bf841f1bbf7b4df0a8ed9d2'
result = dict(status='SIX_CELLS_PASS', spec_sha256=digest, matrix=matrix,
              prior_aarch64_x86_64_builds_not_rerun=True,
              armv7l_check_repetitions_are_not_independent_builds=True)
(p/'bcc_gate.json').write_text(json.dumps(result, ensure_ascii=False, indent=2)+'\n')
print(json.dumps(result, ensure_ascii=False))
