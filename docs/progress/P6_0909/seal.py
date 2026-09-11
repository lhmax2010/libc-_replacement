#!/usr/bin/env python3
"""封存阻塞核查材料；排除后续材料提交／推送动作日志。"""
import datetime
import hashlib
import json
import pathlib
base=pathlib.Path('docs/progress/P6_0909')
start=json.loads((base/'raw/001_resource.time.json').read_text())['start']
now=datetime.datetime.now(datetime.timezone.utc)
elapsed=(now-datetime.datetime.fromisoformat(start)).total_seconds()
assert elapsed<7200
(base/'TIMING.json').write_text(json.dumps({'started':start,'sealed':now.isoformat(),'elapsed_seconds':elapsed,'status':'BLOCKED_REQUIRES_HISTORY_REPLACEMENT'},indent=2)+'\n')
entries=[]
for path in sorted(base.rglob('*')):
    if not path.is_file() or 'delivery' in path.relative_to(base).parts or path.name=='SHA256SUMS':continue
    entries.append(hashlib.sha256(path.read_bytes()).hexdigest()+'  '+str(path.relative_to(base)))
(base/'SHA256SUMS').write_text('\n'.join(entries)+'\n')
print('SEALED_FILES',len(entries),'ELAPSED_SECONDS',round(elapsed,1))
for name in ['snapshot.py','record.py','seal.py']:
    print('SCRIPT_SHA256',hashlib.sha256((base/name).read_bytes()).hexdigest(),name)
