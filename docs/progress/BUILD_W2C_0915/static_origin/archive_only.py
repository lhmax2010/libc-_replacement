"""Copy only the new read-only investigation, not previous package status material."""
import hashlib
import json
import shutil
from pathlib import Path
s=Path('progress/BUILD_W2C_0915/static_origin')
d=Path('docs/progress/BUILD_W2C_0915/static_origin')
copied=[];pending=[]
for path in sorted(s.rglob('*')):
    if not path.is_file():continue
    rel=path.relative_to(s)
    if '__pycache__' in rel.parts:continue
    if rel.parts[0]=='raw' and not (s/'raw'/(path.name.split('.')[0]+'.exitcode')).exists():
        pending.append(str(rel));continue
    target=d/rel
    target.parent.mkdir(parents=True,exist_ok=True)
    if rel.parts[0]=='raw' and target.exists():assert path.read_bytes()==target.read_bytes()
    shutil.copy2(path,target)
    assert hashlib.sha256(path.read_bytes()).digest()==hashlib.sha256(target.read_bytes()).digest()
    copied.append(str(rel))
print(json.dumps(dict(files=len(copied),verified=True,pending_command_records=pending)))
