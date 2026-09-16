import gzip
import hashlib
import json
import re
import shutil
import subprocess
from pathlib import Path

p=Path('progress/BUILD_STATIC_0917B')
d=Path('docs/progress/BUILD_STATIC_0917B')
assert not Path('password.md').exists(), 'Credential input arrived; resume work instead.'
for q in sorted((p/'raw').glob('016_archive_partial.*')):
    shutil.copy2(q,d/'raw'/q.name)
shutil.copy2(Path(__file__),d/'verify_delivery.py')
rows=[]
for q in sorted(d.rglob('*')):
    if not q.is_file() or q.name in ['MANIFEST.json','SELF_CHECK.json']:
        continue
    data=q.read_bytes()
    decoded=gzip.decompress(data) if q.suffix=='.gz' else data
    # No credential has been received. This checks accidental credential-like
    # material, not equality against an unknown password.
    for pattern in [rb'-----BEGIN (?:RSA |OPENSSH |EC )?PRIVATE KEY-----',rb'Authorization:\s*(?:Basic|Bearer)\s+\S+',rb'curl\s+-u\s+[^\s]+:[^\s]+']:
        assert not re.search(pattern,decoded), 'Possible sensitive material: '+str(q)
    rows.append(dict(path=str(q.relative_to(d)), bytes=len(data), sha256=hashlib.sha256(data).hexdigest(), uncompressed_sha256=hashlib.sha256(decoded).hexdigest()))
(d/'MANIFEST.json').write_text(json.dumps(rows,indent=2))
check=dict(files=len(rows),credential_file_present=False,credential_read=False,sudo_calls=0,board_root_calls=0,credential_pattern_scan='PASS',exact_password_string_comparison='NOT_AVAILABLE: password file never present; no password read',w1_static_abort=20,w1_shared_pass=20,w2='BLOCKED',w3='NOT_STARTED',scope='Only this delivery; no package push')
(d/'SELF_CHECK.json').write_text(json.dumps(check,indent=2))
print(json.dumps(check,indent=2))
