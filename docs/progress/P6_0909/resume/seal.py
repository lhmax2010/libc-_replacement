#!/usr/bin/env python3
"""封存续轮元数据与材料证据；原 P6 阻塞记录不改写。"""
import csv
import datetime
import hashlib
import json
import pathlib
base=pathlib.Path('docs/progress/P6_0909/resume')
start=json.loads((base/'raw/001_resource.time.json').read_text())['start']
now=datetime.datetime.now(datetime.timezone.utc)
elapsed=(now-datetime.datetime.fromisoformat(start)).total_seconds()
assert elapsed<7200
(base/'TIMING.json').write_text(json.dumps({'started_utc':start,'sealed_utc':now.isoformat(),'elapsed_seconds':elapsed,'implementation_status':'PUSHED_TREE_IDENTICAL','reference_status':'CURRENT_UPDATED_HISTORICAL_EVIDENCE_PRESERVED_FOR_REVIEW'},indent=2)+'\n')
for source,dest in [('raw/003_remote_before.stdout','NAMESPACE_BEFORE.tsv'),('raw/014_remote_after.stdout','NAMESPACE_AFTER.tsv')]:
    rows=[x.split() for x in (base/source).read_text().splitlines() if 'refs/heads/sandbox/lhmax2025/' in x]
    assert len(rows)==6
    with (base/dest).open('w') as f:
        w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['sha','ref']);w.writerows(rows)
entries=[]
for path in sorted(base.rglob('*')):
    if not path.is_file() or 'delivery' in path.relative_to(base).parts or path in [base/'SHA256SUMS',base/'EVIDENCE_INDEX.tsv']:continue
    entries.append([str(path.relative_to(base)),path.stat().st_size,hashlib.sha256(path.read_bytes()).hexdigest()])
with (base/'EVIDENCE_INDEX.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['path','bytes','sha256']);w.writerows(entries)
(base/'SHA256SUMS').write_text(''.join(f'{digest}  {name}\n' for name,size,digest in entries))
print('SEALED_FILES',len(entries),'ELAPSED_SECONDS',round(elapsed,1))
for path in sorted(base.glob('*.py')):
    print('SCRIPT_SHA256',hashlib.sha256(path.read_bytes()).hexdigest(),path)
