"""Archive only this task; preserve all pre-existing raw evidence bytes."""
import hashlib
import json
import shutil
from pathlib import Path

source=Path('progress/BUILD_W2C_0915')
target=Path('docs/progress/BUILD_W2C_0915')
target.mkdir(parents=True,exist_ok=True)
files=[]
excluded=[]
for path in sorted(source.rglob('*')):
    if not path.is_file():continue
    relative=path.relative_to(source)
    if '__pycache__' in relative.parts:continue
    # An executing wrapper is not a complete command record. Keep it locally;
    # a subsequent receipt archive can include it after its exit code exists.
    if relative.parts[0]=='raw':
        label=path.name.split('.')[0]
        if not (source/'raw'/f'{label}.exitcode').exists():
            excluded.append(str(relative));continue
    data=path.read_bytes()
    dest=target/relative
    if relative.parts[0]=='raw' and dest.exists():
        assert dest.read_bytes()==data, f'Existing raw evidence differs: {relative}'
    dest.parent.mkdir(parents=True,exist_ok=True)
    shutil.copy2(path,dest)
    files.append((str(relative),hashlib.sha256(data).hexdigest(),len(data)))
for name,sha,size in files:
    assert hashlib.sha256((target/name).read_bytes()).hexdigest()==sha
print(json.dumps(dict(status='ARCHIVE_BYTES_VERIFIED',files=len(files),bytes=sum(item[2] for item in files),
    excluded_in_progress_records=excluded),ensure_ascii=False))
