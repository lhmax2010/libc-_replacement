import hashlib,shutil,sys
from pathlib import Path
src=Path('progress/BUILD_NIGHT_0917'); dst=Path('docs/progress/BUILD_NIGHT_0917'); active=sys.argv[1]
snap=src/'candidate_specs'; snap.mkdir(exist_ok=True)
for p in Path('tmp/NIGHT_0917/specs').glob('*'):
    if p.is_file(): shutil.copy2(p,snap/p.name)
rows=[]
for p in sorted(src.rglob('*')):
    if not p.is_file() or p.name=='SHA256SUMS.tsv': continue
    rel=p.relative_to(src)
    if rel.parts[0]=='raw' and p.name.startswith(active+'.'): continue
    q=dst/rel; q.parent.mkdir(parents=True,exist_ok=True); shutil.copy2(p,q)
    data=p.read_bytes(); assert q.read_bytes()==data
    rows.append((str(rel),len(data),hashlib.sha256(data).hexdigest()))
data='path\tbytes\tsha256\n'+''.join(f'{p}\t{s}\t{h}\n' for p,s,h in rows)
(src/'SHA256SUMS.tsv').write_text(data); (dst/'SHA256SUMS.tsv').write_text(data)
print('Archived',len(rows),'files; binary assets remain in tmp; active wrapper excluded until next phase.')
