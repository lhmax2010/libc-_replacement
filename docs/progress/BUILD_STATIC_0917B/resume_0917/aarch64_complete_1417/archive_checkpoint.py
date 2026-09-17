"""Archive completed records and explicitly labeled in-flight snapshots."""
import datetime, hashlib, json, re
from pathlib import Path
src=Path('progress/BUILD_STATIC_0917B/resume_0917')
dst=Path('docs/progress/BUILD_STATIC_0917B/resume_0917')
assert not Path('password.md').exists()
rows=[]
def copy(q,rel):
    data=q.read_bytes()
    for pattern in [rb'-----BEGIN (?:RSA |OPENSSH |EC )?PRIVATE KEY-----',rb'Authorization:\s*(?:Basic|Bearer)\s+\S+',rb'curl\s+-u\s+[^\s]+:[^\s]+']:
        assert not re.search(pattern,data),str(q)
    d=dst/rel
    d.parent.mkdir(parents=True,exist_ok=True)
    if d.exists(): assert d.read_bytes()==data,str(d)
    else: d.write_bytes(data)
    rows.append(dict(source=str(q),delivery=str(d),bytes=len(data),sha256=hashlib.sha256(data).hexdigest()))
for q in sorted(src.iterdir()):
    if not q.is_file(): continue
    if q.name in ['events.jsonl','llvm-aarch64.log','HOURLY_STATUS.md']:
        copy(q,Path('checkpoint_0948')/q.name)
    else: copy(q,Path(q.name))
for q in sorted((src/'raw').iterdir()):
    prefix=q.name.split('.')[0]
    if (q.parent/(prefix+'.finished.txt')).exists(): copy(q,Path('raw')/q.name)
(dst/'CHECKPOINT_MANIFEST.json').write_text(json.dumps(dict(captured_at=datetime.datetime.now().astimezone().isoformat(),status='RUNNING_NOT_RPM_COMPLETE',completed_raw_only=True,live_logs_labeled='checkpoint_0948',files=rows),indent=2))
print(json.dumps(dict(files=len(rows),status='RUNNING_NOT_RPM_COMPLETE',sensitive_pattern_scan='PASS')))
