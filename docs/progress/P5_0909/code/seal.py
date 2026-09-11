#!/usr/bin/env python3
"""核验交付副本，封存非交付动作日志的材料与脚本 SHA256；不包含自身正在写入的运行记录。"""
import csv
import datetime
import hashlib
import json
import pathlib
import subprocess

base=pathlib.Path('docs/progress/P5_0909')
reports=sorted((base/'reports').glob('*.md'))
assert len(reports)==6
for path in reports:
    assert path.read_bytes()==(pathlib.Path('HQ_files')/path.name).read_bytes()
assert subprocess.check_output(['git','branch','--show-current'],text=True).strip()=='codex/runtime-validation'
expected={'docs/progress/R115/W3/raw/033_angle_files.stderr':'94f75a90010d49a676335c1caca88ff04cb1b0a1294bdbb655f38d865eb7a18c',
          'docs/progress/R115/W3/raw/033_angle_files.stdout':'dde4c0b37000f6e3d01bd5096bba7ee331a31b216bd43dcbee24f7f2a9a9cf7c'}
for name,digest in expected.items():
    assert hashlib.sha256(pathlib.Path(name).read_bytes()).hexdigest()==digest,(name,'preexisting file changed')
start=json.loads((base/'raw/001_resource.time.json').read_text())['start']
end=datetime.datetime.now(datetime.timezone.utc)
elapsed=(end-datetime.datetime.fromisoformat(start)).total_seconds()
assert elapsed < 3*3600
(base/'TIMING.json').write_text(json.dumps({'resource_start_utc':start,'materials_sealed_utc':end.isoformat(),'elapsed_seconds_to_seal':elapsed,'time_limit_seconds':10800},indent=2)+'\n')
entries=[]
for path in sorted(base.rglob('*')):
    if not path.is_file() or 'delivery' in path.relative_to(base).parts or path.name in ['SHA256SUMS','EVIDENCE_INDEX.tsv']:continue
    entries.append([str(path.relative_to(base)),path.stat().st_size,hashlib.sha256(path.read_bytes()).hexdigest()])
with (base/'EVIDENCE_INDEX.tsv').open('w') as stream:
    w=csv.writer(stream,delimiter='\t',lineterminator='\n');w.writerow(['path','bytes','sha256']);w.writerows(entries)
(base/'SHA256SUMS').write_text(''.join(f'{digest}  {name}\n' for name,size,digest in entries))
print('SEALED',len(entries),'FILES; SIX_DELIVERY_COPIES_MATCH; PREEXISTING_R115_UNCHANGED; ELAPSED_SECONDS',round(elapsed,1))
for path in sorted((base/'code').glob('*.py')):
    print('SCRIPT_SHA256',hashlib.sha256(path.read_bytes()).hexdigest(),path)
