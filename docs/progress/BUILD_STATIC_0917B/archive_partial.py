import gzip
import hashlib
import json
import shutil
from pathlib import Path

src=Path('progress/BUILD_STATIC_0917B')
dst=Path('docs/progress/BUILD_STATIC_0917B')
assert not Path('password.md').exists(), 'Credential input has arrived; stop partial delivery and resume authorized work.'
assert not dst.exists(), 'Do not overwrite an existing delivery.'
rows=[]
for q in sorted(src.rglob('*')):
    if not q.is_file() or '__pycache__' in q.parts:
        continue
    relative=q.relative_to(src)
    target=dst/relative
    target.parent.mkdir(parents=True, exist_ok=True)
    data=q.read_bytes()
    if len(data)>2_000_000:
        target=target.with_suffix(target.suffix+'.gz')
        with target.open('wb') as f:
            f.write(gzip.compress(data,mtime=0))
    else:
        shutil.copy2(q,target)
    rows.append(dict(source=str(q), delivery=str(target), bytes=len(data), sha256=hashlib.sha256(data).hexdigest(), delivery_sha256=hashlib.sha256(target.read_bytes()).hexdigest()))
(dst/'MANIFEST.json').write_text(json.dumps(rows,indent=2))
matrix=json.loads((src/'w1_matrix.json').read_text())
assert len(matrix)==40
assert all(r['exitcode']==134 for r in matrix if r['mode']=='static-gcc')
assert all(r['cancellation_assertions'] for r in matrix if r['mode']=='shared-gcc')
print('Archived',len(rows),'files; W1 matrix verified. Password file absent; no credential content read or copied.')
