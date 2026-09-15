"""Check staging isolation, authored whitespace and obvious secret-bearing forms."""
import hashlib
import json
import re
import subprocess
from pathlib import Path

p=Path('progress/BUILD_W2C_0915')
root='docs/progress/BUILD_W2C_0915/'
def git(*args):return subprocess.check_output(['git',*args])
files=git('diff','--cached','--name-only','-z').decode().split('\0')
files=[name for name in files if name]
assert files and all(name.startswith(root) for name in files)
assert git('branch','--show-current').decode().strip()=='codex/r5-ehabi-diagnosis'
assert not git('-C','codes/R104/bpftrace','diff','--cached','--name-only').strip()
secret_patterns=[rb'-----BEGIN (?:RSA |OPENSSH |EC )?PRIVATE KEY-----',
                 rb'https?://[^\s/@:]+:[^\s/@]+@',
                 rb'(?im)^\s*(?:Authorization|Cookie|Set-Cookie):\s+\S+']
hits=[]
authored=[]
for name in files:
    data=git('show',':'+name)
    assert data==Path(name).read_bytes(),name
    for pattern in secret_patterns:
        if re.search(pattern,data):hits.append(name)
    # Raw compiler/log output is preserved byte-for-byte, not whitespace-cleaned.
    relative=Path(name).relative_to(root)
    if len(relative.parts)==1 and relative.suffix in ['.md','.py','.sh']:
        authored.append(name)
assert not hits,'Potential secrets found in staged paths: '+','.join(sorted(set(hits)))
r=subprocess.run(['git','diff','--cached','--check','--',*authored],capture_output=True,text=True)
assert r.returncode==0,r.stdout+r.stderr
result=dict(status='PASS',staged_files=len(files),scope=root,authored_whitespace_files=len(authored),
    secret_pattern_hits=0,qualification='Pattern scan only; no credentials files read; not a universal secret detector.')
(p/'staged_delivery_check.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(result))
