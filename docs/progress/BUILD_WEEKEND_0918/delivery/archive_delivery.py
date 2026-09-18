"""Close delivery receipts after stage pushes; no self-referential commit claim."""
import datetime,gzip,hashlib,json
from pathlib import Path
src=Path('progress/BUILD_WEEKEND_0918'); top=Path('docs/progress/BUILD_WEEKEND_0918')
dst=top/'delivery'; dst.mkdir(exist_ok=False)
selected=[]
for label in ('W1','W2','W3','W4'): selected.append(src/(label+'-project-delivery.json'))
selected += [src/'FINAL_SELFCHECK.json',src/'STAGED_CHECK.json',src/'archive_delivery.py',src/'guard-version-preservation.json']
for f in (src/'raw').iterdir():
    if f.is_file() and int(f.name.split('_')[0])>=581:
        prefix=f.name.split('.')[0]
        if (f.parent/(prefix+'.finished.txt')).exists(): selected.append(f)
selected += list((src/'cells/archive-w4').glob('*'))
rows=[]
for f in selected:
    if not f.is_file(): continue
    data=f.read_bytes(); rel=f.relative_to(src); target=dst/rel; zipped=len(data)>524288
    if zipped: target=target.with_name(target.name+'.gz')
    target.parent.mkdir(parents=True,exist_ok=True); target.write_bytes(gzip.compress(data,mtime=0) if zipped else data)
    rows.append({'source':str(f),'delivery':str(target),'sha256_uncompressed':hashlib.sha256(data).hexdigest(),'bytes_uncompressed':len(data),'gzip':zipped})
(dst/'MANIFEST.json').write_text(json.dumps({'time':datetime.datetime.now().astimezone().isoformat(),'files':rows},indent=2))
(top/'STATUS.md').write_bytes((src/'STATUS.md').read_bytes())
(top/'QUESTIONS.md').write_bytes((src/'QUESTIONS.md').read_bytes())
print('Delivery receipts archived',len(rows),'No claim to include this receipt commit or its future push in its own contents')
