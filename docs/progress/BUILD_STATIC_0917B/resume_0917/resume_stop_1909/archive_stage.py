"""One-shot evidence snapshot; never replace an earlier execution's raw files."""
import datetime
import hashlib
import json
import re
import sys
from pathlib import Path

src=Path('progress/BUILD_STATIC_0917B/resume_0917')
dst=Path('docs/progress/BUILD_STATIC_0917B/resume_0917')
label=sys.argv[1]
assert re.fullmatch(r'[a-z0-9_]+',label)
stage=dst/label
assert not stage.exists() and not Path('password.md').exists()
rows=[]
def copy(f,target,mutable=False):
    data=f.read_bytes()
    for pattern in [rb'-----BEGIN (?:RSA |OPENSSH |EC )?PRIVATE KEY-----',rb'Authorization:\s*(?:Basic|Bearer)\s+\S+',rb'curl\s+-u\s+[^\s]+:[^\s]+']:
        assert not re.search(pattern,data),str(f)
    target.parent.mkdir(parents=True,exist_ok=True)
    if target.exists() and not mutable:
        assert target.read_bytes()==data, str(target)
    else:
        target.write_bytes(data)
    rows.append({'source':str(f),'delivery':str(target),'sha256':hashlib.sha256(data).hexdigest(),'bytes':len(data)})
for f in sorted(src.iterdir()):
    if f.is_file() and f.suffix in ('.py','.md','.json','.tsv','.strace'):
        copy(f,stage/f.name)
for f in sorted((src/'raw').iterdir()):
    prefix=f.name.split('.')[0]
    if (f.parent/(prefix+'.finished.txt')).exists(): copy(f,dst/'raw'/f.name)
# The interrupted copy has no exit code. Preserve its partial records as such,
# rather than inventing completion or dropping the interrupted execution.
for f in sorted((src/'raw').glob('269_copy_aarch_inputs.*')):
    if not (src/'raw/269_copy_aarch_inputs.finished.txt').exists():
        copy(f,stage/'interrupted_raw'/f.name)
for dirname in ['arm_llvm','verify_armv7l','cell_commands',*sys.argv[2:]]:
    parent=src/dirname
    if not parent.exists(): continue
    for f in sorted(parent.rglob('*')):
        if f.is_file() and '__pycache__' not in f.parts:
            copy(f,stage/dirname/f.relative_to(parent))
helper=Path('tmp/STATIC_0917B/board_run_bounded.c')
if helper.exists(): copy(helper,stage/'prepared_test_harness'/helper.name)
copy(src/'STATUS.md',dst/'STATUS.md',mutable=True)
(stage/'MANIFEST.json').write_text(json.dumps({'time':datetime.datetime.now().astimezone().isoformat(),
    'note':'Prepared scripts are not proof of execution. Consult literal command records and exit codes.',
    'files':rows},indent=2))
print(label,'files',len(rows))
