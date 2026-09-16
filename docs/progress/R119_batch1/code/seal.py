"""封存当前检查点；已完成记录才纳入，不改变既有证据。"""
from common import *
import json,sys
manifest=OUT/'SHA256.json'
if '--verify' in sys.argv:
    rows=json.loads(manifest.read_text());bad=[r['path']for r in rows if digest(OUT/r['path'])!=r['sha256']]
    print(json.dumps(dict(files=len(rows),mismatches=bad)));raise SystemExit(bool(bad))
rows=[]
for p in sorted(OUT.rglob('*')):
    if not p.is_file() or p==manifest or '__pycache__' in p.parts:continue
    if '/raw/' in str(p):
        suffix=next((s for s in ['.command.txt','.stdout','.stderr','.exitcode','.time.json']if p.name.endswith(s)),None)
        if suffix and not pathlib.Path(str(p)[:-len(suffix)]+'.time.json').is_file():continue
    rows.append(dict(path=str(p.relative_to(OUT)),sha256=digest(p)))
save(manifest,rows);print('sealed_files',len(rows))
