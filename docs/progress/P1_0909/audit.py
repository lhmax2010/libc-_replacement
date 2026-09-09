#!/usr/bin/env python3
"""P1 只读证据核对；不构建、不运行板端测试、不修改输入。"""
import csv
import hashlib
from pathlib import Path

base = Path('docs/progress/IMPL_0908/W3')
for name in ('matrix_x86_64_final.tsv', 'matrix_armv7l_final.tsv', 'timed_versions.tsv'):
    rows = list(csv.DictReader((base / name).open(), delimiter='\t'))
    assert all(r['value_assert'] == 'PASS' for r in rows)
    print(name, 'rows=', len(rows), 'all_recorded_value_assertions=PASS')
    if 'cell' in rows[0]:
        cells = {r['cell'] for r in rows}
        assert len(cells) == 45 and all(sum(r['cell'] == c for r in rows) == 5 for c in cells)
        for r in rows:
            if r['repeat'] == '1' and r['cell'] in ('8', '9', '10', '11'):
                print('binding_row', r)
rows = list(csv.DictReader((base / 'ARM_UNOBSERVED.tsv').open(), delimiter='\t'))
assert len(rows) == 3554
print('ARM_UNOBSERVED rows=', len(rows))
board = Path('docs/progress/P1_0909/raw/007_board_residuals.stdout').read_text()
assert '/opt/usr/impl_0908_lit/runs/run-c5b2ed4b465045ae84201ce5bc79aeaf' in board
assert board.count('CWD_READ_RC=0') == 8
print('diagnostic ZIP cwd attribution: 8; historical positive control present')
proc = Path('docs/progress/P1_0909/raw/010_board_process_links.stdout').read_text()
links = [s for s in proc.splitlines() if s.startswith('PID=')]
assert any('EXE=/usr/bin/bash' in s for s in links)
matches = [s for s in links if any(p in s for p in ('/var/tmp/impl_0908', '/opt/usr/impl_0908_lit'))]
print('process EXE/CWD task-path matches=', len(matches), 'positive control: /usr/bin/bash present')
print('Scope: readable EXE/CWD at one snapshot only; session ownership/reservations NOT_OBSERVED')
for p in [Path(__file__), Path('docs/progress/R114/code/record.py'), Path('tools/resource_gate.sh'),
          Path('tmp/IMPL_0908/source/libcxx/test/std/localization/locales/locale.convenience/conversions/conversions.buffer/overflow.pass.cpp'),
          Path('tmp/IMPL_0908/source/libcxx/test/support/test_macros.h'),
          Path('/usr/include/c++/13/bits/locale_conv.h')]:
    print(hashlib.sha256(p.read_bytes()).hexdigest(), p)
