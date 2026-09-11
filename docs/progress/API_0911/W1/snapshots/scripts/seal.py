#!/usr/bin/env python3
"""对一个阶段的脚本最终快照、证据摘要和用时封存；交付回执另存。"""
import pathlib, hashlib, sys, shutil, datetime, json
root=pathlib.Path('docs/progress/API_0911'); out=root/sys.argv[1]
snap=out/'snapshots/scripts';snap.mkdir(parents=True,exist_ok=True)
for p in [*out.glob('*.py'),*out.glob('*.cpp'),*out.glob('*.h'),root/'record.py',root/'seal.py']:
    shutil.copyfile(p,snap/p.name)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
scripts=sorted(p for p in snap.rglob('*') if p.is_file())
(out/'SCRIPT_SHA256SUMS').write_text(''.join(sha(p)+'  '+str(p.relative_to(out))+'\n' for p in scripts))
files=sorted(p for p in out.rglob('*') if p.is_file() and p.name not in {'SHA256SUMS'} and '__pycache__' not in p.parts and 'delivery' not in p.parts and not p.name.startswith('seal_run.'))
(out/'SHA256SUMS').write_text(''.join(sha(p)+'  '+str(p.relative_to(out))+'\n' for p in files))
print('FILES',len(files),'SCRIPTS',len(scripts))
starts=[]
for p in (out/'raw').glob('*.time.json'):
    starts.append(datetime.datetime.fromisoformat(json.loads(p.read_text())['start']))
if starts: print('ELAPSED_MINUTES',round((datetime.datetime.now(datetime.timezone.utc)-min(starts)).total_seconds()/60,2))
