"""Index existing local W3 reference material; do not connect to a board or run probes."""
import hashlib,json
from pathlib import Path
p=Path.cwd(); out=p/'progress/BPF_W1_0921'
old=p/'progress/BUILD_WEEKEND_0918/arm_cancel_probe_inputs.json'
d=json.loads(old.read_text()); rows=[]
items=[dict(kind='probe_source',path=d['source'],expected=d['source_sha256'])]
items += [dict(kind=x['mode'],path=x['binary'],expected=x['sha256']) for x in d['probes']]
items += [dict(kind='original_arm_asset',path=str(p/'tmp/NIGHT_0917/original_assets/prebuilt-static-bpftrace/arm/usr/bin/bpftrace'),expected='ade7530b3824dd83eebba837928d3919dcb4cd7708212a8607c3629cb5c55f5b')]
for x in items:
    f=Path(x['path']); row=dict(x)
    if f.is_file():
        with f.open('rb') as s: digest=hashlib.file_digest(s,'sha256').hexdigest()
        row.update(sha256=digest,bytes=f.stat().st_size,status='IDENTITY_MATCH' if digest==x['expected'] else 'IDENTITY_DIFFERENT')
    else: row['status']='NOT_AVAILABLE'; row['tried']='checked exact previously recorded local path'
    rows.append(row)
(out/'W3_REFERENCE_INPUTS.json').write_text(json.dumps(dict(source_manifest=str(old),scope='local identities only; no new probe or board execution',rows=rows),indent=2))
print(json.dumps(rows,indent=2))
