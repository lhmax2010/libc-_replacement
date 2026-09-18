"""Archive a completed stage without replacing its original execution records."""
import datetime,gzip,hashlib,json,re,sys
from pathlib import Path
src=Path('progress/BUILD_WEEKEND_0918'); dst=Path('docs/progress/BUILD_WEEKEND_0918')
label=sys.argv[1]; assert re.fullmatch(r'[a-z0-9_]+',label)
stage=dst/label; assert not stage.exists()
stage.mkdir(parents=True)
rows=[]
for f in sorted(src.rglob('*')):
    if not f.is_file() or '__pycache__' in f.parts: continue
    if 'raw' in f.parts:
        prefix=f.name.split('.')[0]
        if not (f.parent/(prefix+'.finished.txt')).exists(): continue
    data=f.read_bytes()
    for pattern in (rb'-----BEGIN (?:RSA |OPENSSH |EC )?PRIVATE KEY-----',rb'Authorization:\s*(?:Basic|Bearer)\s+\S+',rb'curl\s+-u\s+[^\s]+:[^\s]+'):
        assert not re.search(pattern,data),str(f)
    rel=f.relative_to(src); target=stage/rel
    compressed=len(data)>524288
    if compressed: target=target.with_name(target.name+'.gz')
    target.parent.mkdir(parents=True,exist_ok=True)
    target.write_bytes(gzip.compress(data,mtime=0) if compressed else data)
    rows.append({'source':str(f),'delivery':str(target),'sha256_uncompressed':hashlib.sha256(data).hexdigest(),
                 'bytes_uncompressed':len(data),'gzip':compressed})
(stage/'MANIFEST.json').write_text(json.dumps({'time':datetime.datetime.now().astimezone().isoformat(),'files':rows},indent=2))
for name in ('STATUS.md','QUESTIONS.md'):
    (dst/name).write_bytes((src/name).read_bytes())
print(label,'archived_files',len(rows))
