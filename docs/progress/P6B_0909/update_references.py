#!/usr/bin/env python3
"""显式白名单的分支引用机械更新；不改提交 SHA、测试数字或历史日志。"""
import csv
import hashlib
import os
import pathlib

ROOT = pathlib.Path.cwd()
OUT = ROOT / 'docs/progress/P6B_0909'
OLD = 'libcxx-noexcept-relief'
NEW = 'libcxx-ehabi-backport'
CURRENT = [
    'docs/progress/IMPL_0908/W4/SUPPORT_SCOPE_EN.md',
    'docs/progress/IMPL_0908/W4/SUPPORT_SCOPE_ZH.md',
    'docs/progress/IMPL_0909/W3/SUPPORT_SCOPE_EN.md',
    'docs/progress/IMPL_0909/W3/SUPPORT_SCOPE_ZH.md',
    'docs/progress/IMPL_0909/W4/DELIVERY_EN.md',
    'docs/progress/IMPL_0909/W4/DELIVERY_ZH.md',
    'docs/progress/IMPL_STATUS_0908.md',
    'docs/progress/P1_0909/REPORT.md',
]
HISTORY_PARAGRAPHS = ['docs/progress/IMPL_0908/W1/REPORT.md', 'docs/progress/IMPL_0908/W2/REPORT.md']
EXTRA = ['docs/progress/IMPL_0909/W4/REPORT.md', 'docs/progress/IMPL_STATUS_0909.md']
INDEX = 'docs/progress/IMPL_0909/W4/EVIDENCE_INDEX.tsv'

def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()

def tsv(name, header, rows):
    with (OUT / name).open('w', newline='') as f:
        w = csv.writer(f, delimiter='\t', lineterminator='\n')
        w.writerow(header)
        w.writerows(rows)

rows = list(csv.DictReader((OUT / 'BRANCH_REFERENCES_before.tsv').open(), delimiter='\t'))
assert {r['path'] for r in rows if r['category'] == 'CURRENT_REVIEW'} == set(CURRENT + HISTORY_PARAGRAPHS)
historical = sorted({r['path'] for r in rows if r['category'] == 'HISTORY_CANDIDATE'})
tsv('HISTORICAL_FILE_HASHES.tsv', ['path','sha256','reason'], [(p, sha(ROOT/p), '历史原始记录/旧快照/元数据重建审计；保留原文，统一见 BRANCH_MAPPING.md') for p in historical])
edits = []
for name in CURRENT + HISTORY_PARAGRAPHS + EXTRA + [INDEX]:
    p = ROOT / name
    snapshot = OUT / 'before_current' / name
    snapshot.parent.mkdir(parents=True, exist_ok=True)
    assert not snapshot.exists()
    snapshot.write_bytes(p.read_bytes())
    if name == INDEX:
        continue
    text = p.read_text()
    if name in CURRENT:
        assert text.count(OLD) == 1
        for line, value in enumerate(text.splitlines(), 1):
            if OLD in value:
                edits.append([name, line, 'CURRENT_BRANCH_REPLACEMENT', value, value.replace(OLD, NEW)])
        text = text.replace(OLD, NEW)
    english = name.endswith('_EN.md')
    link = os.path.relpath(OUT / 'BRANCH_MAPPING.md', p.parent)
    if english:
        note = (f'> Branch consolidation, 2026-09-11: the current LLVM implementation branch is '
                f'`sandbox/lhmax2025/{NEW}`. The same five commits, trees and Change-Ids are retained; '
                f'no tests were rerun. Historical logs describe the branch used at the time, not a current dependency. '
                f'[Branch mapping and verification]({link}).\n\n')
    else:
        note = (f'> 2026-09-11 分支归并：当前 LLVM 实施分支为 `sandbox/lhmax2025/{NEW}`；'
                f'五个提交的 SHA、tree 与 Change-Id 不变，没有重跑测试。历史记录中的分支名仅说明当时操作，'
                f'不再是当前取件依赖。[分支映射与核验]({link})。\n\n')
    if name in HISTORY_PARAGRAPHS:
        note += '> 下文建立分支／推送到旧分支的叙述是原阶段操作记录，保留原文；当前取件分支以上述映射为准。\n\n'
    heading, rest = text.split('\n\n', 1)
    updated = heading + '\n\n' + note + rest
    p.write_text(updated)
    edits.append([name, 3, 'DATED_MAPPING_NOTICE', '', note.strip()])
tsv('REFERENCE_EDITS.tsv', ['path','original_line_or_insert_line','operation','before','after'], edits)
print(f'current_branch_replacements={len(CURRENT)} mapping_notices={len(CURRENT+HISTORY_PARAGRAPHS+EXTRA)} historical_files_preserved={len(historical)} historical_paragraphs_preserved={len(HISTORY_PARAGRAPHS)}')
