"""Recover exact guard bytes only when matching the SHA recorded at execution."""
import hashlib,json
from pathlib import Path
out=Path('progress/BUILD_WEEKEND_0918')
captured=(out/'raw/010_copy_failure_review.stdout.txt').read_text()
marker='"""Resume the existing RPM build; enforce actual cgroup, serial jobs and I/O gates."""'
old=captured[captured.index(marker):].encode()
current=(out/'guard.py').read_bytes()
versions={hashlib.sha256(x).hexdigest():x for x in (old,current)}
arm_window=(out/'guard_arm_w1_window.py')
if arm_window.exists():
    data=arm_window.read_bytes()
    versions[hashlib.sha256(data).hexdigest()]=data
rows=[]
for f in sorted((out/'cells').glob('*/events.jsonl')):
    starts=[json.loads(x) for x in f.read_text().splitlines() if json.loads(x).get('event')=='start']
    for start in starts:
        sha=start['script_sha256']; assert sha in versions,(f,sha)
        target=f.parent/'guard-exact-sha.py'
        if target.exists(): assert target.read_bytes()==versions[sha]
        else: target.write_bytes(versions[sha])
        rows.append({'cell':str(f.parent),'sha256':sha,'matches_execution_record':True,
                     'source':'captured raw/010 full script output' if versions[sha]==old else ('unchanged current guard' if versions[sha]==current else 'W1 ARM shortened-window guard'),
                     'note':'Snapshot saved after execution start; exact bytes match the SHA recorded at start.'})
(out/'guard-version-preservation.json').write_text(json.dumps(rows,indent=2))
print('Exact SHA-matched guard versions',len(rows))
