#!/usr/bin/env bash
set -euo pipefail
set -x

workspace=/home/toolchain/development/libc++_replacement
cd "$workspace"

python3 - <<'PY'
import csv
from pathlib import Path

root = Path('progress/R15')
rows = list(csv.DictReader((root / 'tables/connectivity.tsv').open(), delimiter='\t'))
assert len(rows) == 5
assert sum(row['method'] == 'SSH' and row['observed_result'] == 'CONNECTION_REFUSED' for row in rows) == 2
assert sum(row['method'] == 'SDB' and row['observed_result'] == 'FAILED_TO_CONNECT' for row in rows) == 2
assert sum(row['method'] == 'PING' and row['exit_code'] == '0' for row in rows) == 1
for log in root.joinpath('commands').glob('*.log'):
    if log.name.startswith('08_final_audit'):
        continue
    text = log.read_text(errors='replace')
    assert 'COMMAND=' in text, log
    assert 'EXIT_CODE=' in text, log
feasibility = list(csv.DictReader((root / 'tables/feasibility.tsv').open(), delimiter='\t'))
overall = [row for row in feasibility if row['question'] == 'overall']
assert len(overall) == 1
assert overall[0]['status'] == 'NOT_FEASIBLE'
report = (root / 'R15_report.md').read_text()
for required in (
    'NOT_FEASIBLE_CURRENT_SESSION_NO_EXECUTION_CARRIER',
    'Connection refused',
    'NOT_AVAILABLE',
    '没有在板上 touch 文件',
    '不提供方案建议',
):
    assert required in report, required
print('R15_ASSERTIONS=PASS')
print(f'R15_COMMAND_LOGS_BEFORE_CURRENT={len([p for p in root.joinpath("commands").glob("*.log") if not p.name.startswith("08_final_audit")])}')
PY

git diff --exit-code -- codes packaging artifacts
git status --porcelain -- codes packaging artifacts
find progress/R15 -type f -printf '%s\t%p\n' | sort -nr | sed -n '1,20p'
