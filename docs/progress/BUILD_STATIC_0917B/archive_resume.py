"""Archive this task's evidence without replacing an existing raw observation."""
import gzip
import hashlib
import json
import re
import shutil
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
    data=q.read_bytes()
    # Password contents were never printed or copied. These patterns do not
    # substitute for an exact secret-string check after the file was shredded.
    for pattern in [rb'-----BEGIN (?:RSA |OPENSSH |EC )?PRIVATE KEY-----',
                    rb'Authorization:\s*(?:Basic|Bearer)\s+\S+',
                    rb'curl\s+-u\s+[^\s]+:[^\s]+']:
        assert not re.search(pattern,data), f'Possible sensitive material in {rel}; stop.'
    target=dst/rel
    if len(data)>2_000_000:
        target=target.with_suffix(target.suffix+'.gz')
        stored=gzip.compress(data,mtime=0)
    else:
        stored=data
    if rel.parts[0]=='raw' and target.exists():
        assert target.read_bytes()==stored, f'Existing raw observation differs: {rel}'
    target.parent.mkdir(parents=True,exist_ok=True)
    target.write_bytes(stored)
    manifest.append(dict(source=str(q),delivery=str(target),bytes=len(data),sha256=hashlib.sha256(data).hexdigest(),delivery_sha256=hashlib.sha256(stored).hexdigest()))
(dst/'RESUME_MANIFEST.json').write_text(json.dumps(manifest,indent=2))
check=dict(credential_file_absent=True,credential_tracked=False,
           secret_like_pattern_scan='PASS',
           exact_password_string_scan='NOT_OBSERVED: password never read into records; source shredded immediately after authorized copy/chown; not retained for comparison',
           sudo_calls=4,board_root_calls=0,
           original_raw_preservation='PASS',files=len(manifest))
(dst/'RESUME_SELF_CHECK.json').write_text(json.dumps(check,indent=2))
print(json.dumps(check,indent=2))
