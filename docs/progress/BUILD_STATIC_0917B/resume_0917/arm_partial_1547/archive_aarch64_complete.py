import datetime
import hashlib
import json
import re
from pathlib import Path

src=Path('progress/BUILD_STATIC_0917B/resume_0917')
dst=Path('docs/progress/BUILD_STATIC_0917B/resume_0917')
stage=dst/'aarch64_complete_1417'
assert not stage.exists() and not Path('password.md').exists()
assert (src/'raw/116_rpm_complete_retry.exitcode').read_text().strip()=='0'
assert json.loads((src/'verify_aarch64/result.json').read_text())['status']=='RPM_PAYLOAD_CHECK_PASS'
rows=[]
def copy(q,d,report=False):
    data=q.read_bytes()
    for pattern in [rb'-----BEGIN (?:RSA |OPENSSH |EC )?PRIVATE KEY-----',rb'Authorization:\s*(?:Basic|Bearer)\s+\S+',rb'curl\s+-u\s+[^\s]+:[^\s]+']:
        assert not re.search(pattern,data),str(q)
    d.parent.mkdir(parents=True,exist_ok=True)
    if d.exists() and not report: assert d.read_bytes()==data,str(d)
    else: d.write_bytes(data)
    rows.append(dict(source=str(q),delivery=str(d),sha256=hashlib.sha256(data).hexdigest(),bytes=len(data)))
for q in sorted(src.iterdir()):
    if q.is_file(): copy(q,stage/q.name)
for q in sorted((src/'raw').iterdir()):
    prefix=q.name.split('.')[0]
    if (q.parent/(prefix+'.finished.txt')).exists(): copy(q,dst/'raw'/q.name)
for q in sorted((src/'verify_aarch64').iterdir()):
    assert q.is_file()
    copy(q,dst/'verify_aarch64'/q.name)
for name in ['STATUS.md','AARCH64_RPM_RESULT.md']:
    copy(src/name,dst/name,report=True)
(stage/'MANIFEST.json').write_text(json.dumps(dict(time=datetime.datetime.now().astimezone().isoformat(),status='AARCH64_LLVM_RPM_PAYLOAD_PASS__BPFTRACE_AND_BOARD_PENDING',files=rows),indent=2))
print('Archived aarch64 completed RPM evidence:',len(rows),'files')
