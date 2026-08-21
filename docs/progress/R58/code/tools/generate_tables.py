#!/usr/bin/env python3
from pathlib import Path
import re

root = Path('/home/toolchain/development/libc++_replacement')
raw = root / 'progress/R58/raw'
tables = root / 'progress/R58/tables'
tables.mkdir(parents=True, exist_ok=True)

def exit_code(path):
    text = path.read_text(errors='replace')
    vals = re.findall(r'^(?:EXIT_CODE|REMOTE_CELL_EXIT_CODE)=([0-9]+)', text, re.M)
    return vals[-1] if vals else 'NOT_OBSERVED'

def table_for(group):
    rows = []
    for arch in ('x86_64', 'armv7l'):
        d = raw / arch / group
        if d.exists():
            for p in sorted(d.glob('*.log')):
                rows.append((arch, p.stem, exit_code(p), str(p.relative_to(root))))
    out = tables / f'{group}_matrix.tsv'
    out.write_text('arch\tcase\texit_code\tevidence\n' + ''.join('\t'.join(r)+'\n' for r in rows))
    return rows

groups = {g: table_for(g) for g in ('layer','facility','async','noncancel','regression','upstream','concurrency')}

lines = ['arch\truntime\tattempts\texit_0\tnonzero\twait_returned\n']
for arch in ('x86_64','armv7l'):
    for rt in ('baseline','full','reference'):
        rows = [r for r in groups['async'] if r[0] == arch and r[1].startswith(rt+'_cancel_')]
        ok = sum(r[2] == '0' for r in rows)
        wait = 0
        for r in rows:
            p = root / r[3]
            if 'WAIT_RETURNED=1' in p.read_text(errors='replace'):
                wait += 1
        lines.append(f'{arch}\t{rt}\t{len(rows)}\t{ok}\t{len(rows)-ok}\t{wait}\n')
(tables/'async_20_run_summary.tsv').write_text(''.join(lines))

lines = ['arch\truntime\tmode\tattempts\texit_0\tnonzero\n']
for arch in ('x86_64','armv7l'):
    for rt in ('full','reference'):
        for mode in ('multi_cancel','cancel_and_exception','stdlib_concurrent','repeat_cancel'):
            rows = [r for r in groups['concurrency'] if r[0] == arch and r[1].startswith(f'{rt}_{mode}_')]
            ok = sum(r[2] == '0' for r in rows)
            lines.append(f'{arch}\t{rt}\t{mode}\t{len(rows)}\t{ok}\t{len(rows)-ok}\n')
(tables/'concurrency_summary.tsv').write_text(''.join(lines))

lines = ['arch\tfacility\tbaseline\tfull\treference\n']
for arch in ('x86_64','armv7l'):
    by = {r[1]: r[2] for r in groups['facility'] if r[0] == arch}
    for mode in ('cout','getline_free','getline_member','read','arithmetic','packaged_task','async'):
        lines.append(f'{arch}\t{mode}\t{by.get("baseline_"+mode,"NOT_OBSERVED")}\t{by.get("full_"+mode,"NOT_OBSERVED")}\t{by.get("reference_"+mode,"NOT_OBSERVED")}\n')
    lines.append(f'{arch}\tbasic_string::shrink_to_fit\tNOT_OBSERVED\tNOT_OBSERVED\tNOT_OBSERVED\n')
    lines.append(f'{arch}\tbasic_ostream<char>::sentry::~sentry\tNOT_OBSERVED\tNOT_OBSERVED\tNOT_OBSERVED\n')
(tables/'facility_summary.tsv').write_text(''.join(lines))

lines = ['arch\tsuite\ttotal\texit_0\tnonzero\n']
for arch in ('x86_64','armv7l'):
    for group in ('regression','upstream'):
        rows = [r for r in groups[group] if r[0] == arch]
        ok = sum(r[2] == '0' for r in rows)
        lines.append(f'{arch}\t{group}\t{len(rows)}\t{ok}\t{len(rows)-ok}\n')
(tables/'regression_summary.tsv').write_text(''.join(lines))

selected = raw/'x86_64/libcxx_selected_matrix.tsv'
(tables/'libcxx_selected_summary.tsv').write_text(selected.read_text() if selected.exists() else 'NOT_AVAILABLE\n')

(tables/'known_not_observed.tsv').write_text(
    'item\tarch\tstatus\treason\n'
    'basic_string::shrink_to_fit\tx86_64,armv7l\tNOT_OBSERVED\tUNREACHABLE_NOEXCEPT\n'
    'basic_ostream<char>::sentry::~sentry\tx86_64,armv7l\tNOT_OBSERVED\tUNREACHABLE_NOEXCEPT\n'
    'selected libc++ source tests\tarmv7l\tNOT_AVAILABLE\t板上无编译器/lit，且本轮未生成同构预编译集合\n'
)
