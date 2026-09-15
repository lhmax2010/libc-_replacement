import datetime,re,json
from pathlib import Path
p=Path('progress/BUILD_NIGHT_0917'); rows=[]
for f in p.glob('libclang-*-build.events.jsonl'):
    records=[json.loads(x) for x in f.read_text().splitlines()]
    last=records[-1]
    log=f.with_name(f.name.replace('.events.jsonl','.build.log'))
    targets=re.findall(r'\[(\d+)/(\d+)\]',log.read_text(errors='replace')) if log.exists() else []
    rows.append(dict(label=f.name,event=last['event'],target='/'.join(targets[-1]) if targets else 'NOT_OBSERVED',exitcode=last.get('exitcode','NOT_OBSERVED')))
now=datetime.datetime.now().astimezone().isoformat()
with (p/'STATUS.md').open('a') as f:
    f.write('\n### 进度记录 '+now+'\n\n')
    for r in rows: f.write(f'- {r["label"]}: {r["event"]}, {r["target"]}, exitcode={r["exitcode"]}.\n')
target=Path('docs/progress/BUILD_NIGHT_0917/STATUS.md')
target.parent.mkdir(parents=True,exist_ok=True)
target.write_bytes((p/'STATUS.md').read_bytes())
print(json.dumps(dict(time=now,rows=rows),indent=2))
