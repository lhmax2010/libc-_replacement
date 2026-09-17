"""封存当前检查点；已完成记录才纳入，不改变既有证据。"""
from common import *
import json,sys
manifest=OUT/'SHA256.json'
if '--verify' in sys.argv:
    rows=json.loads(manifest.read_text());bad=[r['path']for r in rows if digest(OUT/r['path'])!=r['sha256']]
    print(json.dumps(dict(files=len(rows),mismatches=bad)));raise SystemExit(bool(bad))
carry={};unchanged=set()
if '--incremental' in sys.argv and manifest.exists():
    # 已提交且工作区未改的文件复用上一检查点摘要；新增/修改文件仍重新计算。
    carry={r['path']:r['sha256']for r in json.loads(manifest.read_text())}
    scope=str(OUT.relative_to(ROOT))
    rc,tracked,raw=record('seal_tracked_paths',['git','ls-files','--',scope]);assert rc==0
    rc,changed,raw=record('seal_changed_paths',['git','diff','HEAD','--name-only','--',scope]);assert rc==0
    changed=set(changed.splitlines())
    unchanged={str(pathlib.Path(p).relative_to(scope))for p in tracked.splitlines()if p not in changed}
rows=[];reused=0
for p in sorted(OUT.rglob('*')):
    if not p.is_file() or p==manifest or '__pycache__' in p.parts:continue
    if '/raw/' in str(p):
        suffix=next((s for s in ['.command.txt','.stdout','.stderr','.exitcode','.time.json']if p.name.endswith(s)),None)
        if suffix and not pathlib.Path(str(p)[:-len(suffix)]+'.time.json').is_file():continue
    rel=str(p.relative_to(OUT))
    if rel in unchanged and rel in carry:h=carry[rel];reused+=1
    else:h=digest(p)
    rows.append(dict(path=rel,sha256=h))
save(manifest,rows);print('sealed_files',len(rows),'unchanged_committed_hashes_reused',reused)
