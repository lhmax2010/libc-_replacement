"""Keep immutable checkpoint evidence; never overwrite earlier raw logs."""
import datetime
import hashlib
import json
import re
from pathlib import Path

src = Path('progress/BUILD_STATIC_0917B/resume_0917')
dst = Path('docs/progress/BUILD_STATIC_0917B/resume_0917')
snap = dst / 'checkpoint_1346'
assert not snap.exists()
assert not Path('password.md').exists()
rows = []
def copy(q, d):
    data = q.read_bytes()
    for pattern in [rb'-----BEGIN (?:RSA |OPENSSH |EC )?PRIVATE KEY-----',
                    rb'Authorization:\s*(?:Basic|Bearer)\s+\S+',
                    rb'curl\s+-u\s+[^\s]+:[^\s]+']:
        assert not re.search(pattern, data), str(q)
    d.parent.mkdir(parents=True, exist_ok=True)
    if d.exists():
        assert d.read_bytes() == data, str(d)
    else:
        d.write_bytes(data)
    rows.append({'source': str(q), 'delivery': str(d), 'bytes': len(data),
                 'sha256': hashlib.sha256(data).hexdigest()})
for q in sorted(src.iterdir()):
    if q.is_file():
        copy(q, snap / q.name)
for q in sorted((src / 'raw').iterdir()):
    prefix = q.name.split('.')[0]
    if (q.parent / (prefix + '.finished.txt')).exists():
        copy(q, dst / 'raw' / q.name)
(snap / 'MANIFEST.json').write_text(json.dumps({
    'time': datetime.datetime.now().astimezone().isoformat(),
    'status': 'FIRST_BUILD_EXIT_0__DELIVERY_RPM_SET_RETRY_RUNNING',
    'live_logs_are_snapshots': True, 'completed_raw_only': True, 'files': rows}, indent=2))
print('Archived', len(rows), 'records; RPM retry still running, not final delivery.')
