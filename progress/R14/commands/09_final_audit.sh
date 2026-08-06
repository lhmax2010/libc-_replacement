#!/usr/bin/env bash
set -euo pipefail
set -x

workspace=/home/toolchain/development/libc++_replacement
cd "$workspace"

python3 - <<'PY'
import csv
from pathlib import Path

root = Path('progress/R14')
matrix = list(csv.DictReader((root / 'tables/matrix_execution.tsv').open(), delimiter='\t'))
assert len(matrix) == 96
assert all(row['outcome'] == 'SIGNAL_SIGABRT' for row in matrix)
controls = [row for row in matrix if row['direction'] in ('new_new', 'legacy_legacy')]
assert len(controls) == 48
assert all(row['type_check'] == 'NOT_OBSERVED' for row in controls)
assert len(list((root / 'commands/matrix').glob('*.log'))) == 96
assert len(list((root / 'raw/matrix').glob('*/ld_debug.*'))) == 96
assert len(list((root / 'raw/matrix').glob('*/program.stdout'))) == 96
assert len(list((root / 'raw/matrix').glob('*/program.stderr'))) == 96
for log in root.joinpath('commands').rglob('*.log'):
    if log.name.startswith('09_final_audit'):
        continue
    text = log.read_text(errors='replace')
    assert 'COMMAND=' in text, log
    assert 'EXIT_CODE=' in text, log
inputs = list(csv.DictReader((root / 'tables/input_rpm_identity.tsv').open(), delimiter='\t'))
assert len(inputs) == 5
assert all(row['sha_status'] == 'MATCH' for row in inputs)
events = list(csv.DictReader((root / 'tables/program_events.tsv').open(), delimiter='\t'))
assert len(events) == 96
assert all(row['event'].startswith('THROW:') for row in events)
report = (root / 'R14_report.md').read_text()
for required in (
    'RED_STOP_INVALID_SAME_SIDE_CONTROL',
    '96/96',
    '48/48',
    'NOT_OBSERVED_INVALID_SAME_SIDE_CONTROL',
    '不做方案推荐',
):
    assert required in report, required
print('R14_ASSERTIONS=PASS')
print(f'R14_COMMAND_LOGS_BEFORE_CURRENT={len([p for p in root.joinpath("commands").rglob("*.log") if not p.name.startswith("09_final_audit")])}')
print(f'R14_MATRIX_ROWS={len(matrix)}')
print(f'R14_SAME_SIDE_CONTROLS={len(controls)}')
PY

git diff --exit-code -- codes packaging artifacts
git status --porcelain -- codes packaging artifacts
find progress/R14 -type f -printf '%s\t%p\n' | sort -nr | sed -n '1,20p'
