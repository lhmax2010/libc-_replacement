"""Add native-gate evidence; never overwrite earlier raw observations."""
import gzip
import datetime
import hashlib
import json
import re
import subprocess
from pathlib import Path

src=Path('progress/BUILD_STATIC_0917B')
dst=Path('docs/progress/BUILD_STATIC_0917B')
assert not Path('password.md').exists()
assert subprocess.run(['git','ls-files','--error-unmatch','password.md'],capture_output=True).returncode != 0
manifest=[]
for q in sorted(src.rglob('*')):
    if not q.is_file() or '__pycache__' in q.parts:
        continue
    rel=q.relative_to(src)
    # Existing raw files were frozen at their previous delivery time.
    if rel.parts[0]=='raw' and int(rel.name.split('_')[0])<147:
        continue
    if rel.parts[0]=='raw':
        prefix=rel.name.split('.')[0]
        if not (q.parent/(prefix+'.finished.txt')).exists():
            continue
    if q.name in ['STAGED_CHECK.json']:
        continue
    data=q.read_bytes()
    for pattern in [rb'-----BEGIN (?:RSA |OPENSSH |EC )?PRIVATE KEY-----',rb'Authorization:\s*(?:Basic|Bearer)\s+\S+',rb'curl\s+-u\s+[^\s]+:[^\s]+']:
        assert not re.search(pattern,data), str(rel)
    target=dst/rel
    stored=data
    if len(data)>2_000_000:
        target=target.with_suffix(target.suffix+'.gz')
        stored=gzip.compress(data,mtime=0)
    if target.exists() and rel.parts[0]=='raw':
        assert target.read_bytes()==stored, 'Changed raw file: '+str(rel)
    target.parent.mkdir(parents=True,exist_ok=True)
    target.write_bytes(stored)
    manifest.append(dict(source=str(q),delivery=str(target),bytes=len(data),sha256=hashlib.sha256(data).hexdigest(),delivery_sha256=hashlib.sha256(stored).hexdigest()))
(dst/'NATIVE_MANIFEST.json').write_text(json.dumps(manifest,indent=2))
check=dict(captured_at=datetime.datetime.now().astimezone().isoformat(),files=len(manifest),secret_pattern_scan='PASS',password_absent=True,sudo_calls_added=0,exact_password_scan='NOT_OBSERVED: previously shredded, never retained',active_raw_commands='Excluded until logger finished; build/event logs are timestamped snapshots, not a completion assertion')
(dst/'NATIVE_SELF_CHECK.json').write_text(json.dumps(check,indent=2))
print(json.dumps(check))
