#!/usr/bin/env python3
"""盘点全部跟踪文本中的分支旧名，先列出现行/历史候选，不自动修改。"""
import csv
import pathlib
import subprocess
import sys

ROOT = pathlib.Path.cwd()
OUT = ROOT / 'docs/progress/P6B_0909'
NEEDLE = 'libcxx-noexcept-relief'
phase = sys.argv[1]
files = subprocess.check_output(['git', 'ls-files', '-z']).decode().split('\0')
rows = []
texts = 0
for name in files:
    if not name:
        continue
    p = ROOT / name
    if not p.is_file():
        continue
    raw = p.read_bytes()
    if b'\0' in raw:
        continue
    try:
        value = raw.decode('utf-8')
    except UnicodeDecodeError:
        continue
    texts += 1
    if NEEDLE not in value:
        continue
    for line_no, line in enumerate(value.splitlines(), 1):
        if NEEDLE not in line:
            continue
        historic = any(part in {'raw','before','before_current','diffs','delivery','delivery_resume','snapshots','original_commits','commit_objects'} for part in p.parts)
        historic |= any('/' + x + '/' in name for x in ['P6_0909', 'P6B_0909', 'P4_0909', 'P5_0909'])
        category = 'HISTORY_CANDIDATE' if historic else 'CURRENT_REVIEW'
        rows.append([name, line_no, line.count(NEEDLE), category, line[:1800]])
with (OUT / ('BRANCH_REFERENCES_' + phase + '.tsv')).open('w', newline='') as f:
    w = csv.writer(f, delimiter='\t', lineterminator='\n')
    w.writerow(['path','line','occurrences','category','context'])
    w.writerows(rows)
assert NEEDLE in 'sandbox/lhmax2025/libcxx-noexcept-relief'
print(f'text_files={texts} matching_files={len({r[0] for r in rows})} matching_lines={len(rows)} occurrences={sum(r[2] for r in rows)} positive_control=PASS')
for name in sorted({r[0] for r in rows if r[3] == 'CURRENT_REVIEW'}):
    print(name)
