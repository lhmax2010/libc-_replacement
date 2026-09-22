"""Inspect only the staged task documents; never print secret values."""
import base64,hashlib,json,re,subprocess
from pathlib import Path
p=Path.cwd(); out=p/'progress/BPF_LIBBPF_0922'
def git(*args):
    r=subprocess.run(['git',*args],capture_output=True)
    assert r.returncode==0,('git read failed',args)
    return r.stdout
assert git('branch','--show-current').strip()==b'codex/r5-ehabi-diagnosis'
files=[x.decode() for x in git('diff','--cached','--name-only','-z').split(b'\0') if x]
assert files and all(x=='docs/LINE_STATUS.md' or x.startswith('docs/progress/BPF_LIBBPF_0922/') for x in files)
secrets=[]
for name in ('.quickbuild-credentials','password.md'):
    f=p/name
    if f.exists():
        values=f.read_bytes().splitlines()
        if name=='.quickbuild-credentials': values=values[1:]
        for value in values:
            if len(value)>=4: secrets.extend((value,base64.b64encode(value)))
rows=[]
for name in files:
    data=git('show',':'+name)
    assert not any(s in data for s in secrets),('potential credential; STOP',name)
    assert not re.search(rb'-----BEGIN (?:RSA |OPENSSH |EC )?PRIVATE KEY-----|gh[pousr]_[A-Za-z0-9]{30,}',data),('potential secret; STOP',name)
    rows.append(dict(path=name,bytes=len(data),sha256=hashlib.sha256(data).hexdigest()))
result=dict(status='PASS',staged_files=rows,scope='task evidence and LINE_STATUS only',credential_values_printed=False)
(out/'STAGED_CHECK.json').write_text(json.dumps(result,indent=2))
print('STAGED_SCOPE_AND_CREDENTIAL_SCAN_PASS',len(rows),'files')

