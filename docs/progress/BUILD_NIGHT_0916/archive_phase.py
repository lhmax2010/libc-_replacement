"""Archive only this task's completed evidence; preserve all other task material."""
import hashlib
from pathlib import Path
import shutil
import sys

src = Path('progress/BUILD_NIGHT_0916')
dst = Path('docs/progress/BUILD_NIGHT_0916')
active = sys.argv[1] if len(sys.argv)>1 else ''
records=[]
for f in sorted(src.rglob('*')):
    if not f.is_file():
        continue
    rel=f.relative_to(src)
    if f.name == 'SHA256SUMS.tsv' or (rel.parts[0]=='raw' and active and f.name.startswith(active+'.')):
        continue
    target=dst/rel
    target.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(f,target)
    data=f.read_bytes()
    assert target.read_bytes()==data
    records.append((str(rel),len(data),hashlib.sha256(data).hexdigest()))
manifest='path\tbytes\tsha256\n'+''.join(f'{p}\t{s}\t{h}\n' for p,s,h in records)
(src/'SHA256SUMS.tsv').write_text(manifest)
(dst/'SHA256SUMS.tsv').write_text(manifest)
print(f'ARCHIVED {len(records)} files; current wrapper {active} excluded until it completes; no package binaries copied.')
