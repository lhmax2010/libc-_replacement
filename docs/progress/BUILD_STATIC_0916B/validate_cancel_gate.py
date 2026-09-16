import json
from pathlib import Path
rows=json.loads(Path('progress/BUILD_STATIC_0916B/cancel_matrix.json').read_text())
assert len(rows)==40
for mode in ['static','dynamic']:
    cells=[r for r in rows if r['mode']==mode]
    assert len(cells)==20
    print(mode, 'PASS',sum(r['assertions_pass'] for r in cells),'/ 20', 'exitcodes',sorted({r['exitcode'] for r in cells}))
passed=all(r['assertions_pass'] for r in rows)
print('STATIC_CANCELLATION_GATE='+('PASS' if passed else 'FAIL'))
raise SystemExit(0 if passed else 1)
