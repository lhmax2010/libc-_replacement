import hashlib,json,shutil
from pathlib import Path
b=Path('progress/ARM_RPM_DIAG');d=b/'script-snapshots';d.mkdir(exist_ok=False)
rows=[]
for p in sorted(b.glob('*.py')):
    shutil.copy2(p,d/p.name)
    rows.append({'source':str(p),'snapshot':str(d/p.name),'sha256':hashlib.sha256((d/p.name).read_bytes()).hexdigest()})
start=next(json.loads(l) for l in (b/'cells/llvm-arm-repro/events.jsonl').read_text().splitlines() if json.loads(l)['event']=='start')
guard=next(r for r in rows if r['source'].endswith('/guard.py'))
assert guard['sha256']==start['script_sha256']
(b/'script-snapshot-sha256.json').write_text(json.dumps(rows,indent=2)+'\n')
print('Saved',len(rows),'script snapshots; guard SHA256 matches actual launch event:',guard['sha256'])
