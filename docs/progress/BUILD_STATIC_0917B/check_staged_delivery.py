import gzip
import hashlib
import json
import re
import subprocess
from pathlib import Path

assert not Path('password.md').exists()
assert not subprocess.check_output(['git','status','--porcelain','--','password.md']).strip()
paths=subprocess.check_output(['git','diff','--cached','--name-only','-z']).decode().split('\0')
paths=[p for p in paths if p]
assert paths and all(p.startswith('docs/progress/BUILD_STATIC_0917B/') for p in paths)
rows=[]
for p in paths:
    assert Path(p).name not in ['password.md','.quickbuild-credentials']
    data=subprocess.check_output(['git','show',':'+p])
    assert data==Path(p).read_bytes(), p
    decoded=gzip.decompress(data) if p.endswith('.gz') else data
    for pattern in [rb'-----BEGIN (?:RSA |OPENSSH |EC )?PRIVATE KEY-----',rb'Authorization:\s*(?:Basic|Bearer)\s+\S+',rb'curl\s+-u\s+[^\s]+:[^\s]+']:
        assert not re.search(pattern,decoded), 'Possible sensitive material: '+p
    rows.append(dict(path=p,sha256=hashlib.sha256(data).hexdigest(),bytes=len(data)))
result=dict(changed_files=len(rows),scope='Only docs/progress/BUILD_STATIC_0917B',
            credential_file_absent=True,credential_git_status_empty=True,
            indexed_bytes_equal_reviewed_files=True,sensitive_pattern_scan='PASS',
            exact_password_string_comparison='NOT_OBSERVED; secret not retained or read for comparison; no claim of exact-string scan',
            files=rows)
Path('progress/BUILD_STATIC_0917B/STAGED_CHECK.json').write_text(json.dumps(result,indent=2))
print(json.dumps({k:v for k,v in result.items() if k!='files'},indent=2))
