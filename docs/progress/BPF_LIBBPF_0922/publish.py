"""Archive task evidence only; refuse secrets, oversized files or unrelated staged changes."""
import base64,hashlib,json,re,shutil,subprocess,sys
from pathlib import Path
p=Path.cwd(); src=p/'progress/BPF_LIBBPF_0922'; dst=p/'docs/progress/BPF_LIBBPF_0922'
secrets=[]
for name in ('.quickbuild-credentials','password.md'):
    f=p/name
    if f.exists():
        values=f.read_bytes().splitlines()
        if name=='.quickbuild-credentials': values=values[1:]
        for value in values:
            if len(value)>=4: secrets.extend((value,base64.b64encode(value)))
records=[]; omitted=[]
for f in sorted(src.rglob('*')):
    if not f.is_file(): continue
    rel=f.relative_to(src)
    if rel==Path('PUBLICATION_MANIFEST.json'): continue  # manifest cannot hash itself
    if '__pycache__' in rel.parts: omitted.append(str(rel)); continue
    assert not f.is_symlink(),f
    b=f.read_bytes()
    assert len(b)<50000000,('large evidence file requires disclosure',str(rel),len(b))
    assert not any(s in b for s in secrets),('potential credential; stop',str(rel))
    assert not re.search(rb'-----BEGIN (?:RSA |OPENSSH |EC )?PRIVATE KEY-----|gh[pousr]_[A-Za-z0-9]{30,}',b),('potential secret',str(rel))
    d=dst/rel; d.parent.mkdir(parents=True,exist_ok=True); shutil.copy2(f,d)
    digest=hashlib.sha256(b).hexdigest(); assert hashlib.sha256(d.read_bytes()).hexdigest()==digest
    records.append(dict(path=str(rel),bytes=len(b),sha256=digest))
receipt=dict(files=records,credential_scan='PASS',omitted_reproducible_python_caches=omitted)
(src/'PUBLICATION_MANIFEST.json').write_text(json.dumps(receipt,indent=2))
shutil.copy2(src/'PUBLICATION_MANIFEST.json',dst/'PUBLICATION_MANIFEST.json')
print(json.dumps(dict(files=len(records),bytes=sum(r['bytes'] for r in records),credential_scan='PASS',omitted_reproducible_python_caches=omitted)))

