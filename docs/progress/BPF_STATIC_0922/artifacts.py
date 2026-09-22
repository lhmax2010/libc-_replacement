"""Generate candidate recipe snapshots and inventory from actual completed cells."""
import csv,difflib,hashlib,json,shutil
from pathlib import Path
P=Path.cwd(); E=P/'progress/BPF_STATIC_0922'; T=P/'tmp/BPF_STATIC_0922'
def sha(f):
    with f.open('rb') as s:return hashlib.file_digest(s,'sha256').hexdigest()
(E/'recipe').mkdir(exist_ok=True)
src=P/'tmp/BPF_W1_0921/specs/bpftrace.spec'; new=T/'specs/bpftrace.spec'
patch=T/'sources-armv7l/static-link-libbpf.patch'
for f in (new,patch):shutil.copy2(f,E/'recipe'/f.name)
diff=''.join(difflib.unified_diff(src.read_text().splitlines(True),new.read_text().splitlines(True),fromfile='a/packaging/bpftrace.spec',tofile='b/packaging/bpftrace.spec'))
(E/'SPEC_DIFF.patch').write_text(diff)
(E/'recipe/IDENTITY.json').write_text(json.dumps([dict(path=str(f),sha256=sha(f)) for f in (src,new,patch)],indent=2))
rows=[]; req=[]
for arch in ('armv7l','aarch64'):
    result=E/f'verify-{arch}/result.json'
    if not result.exists():continue
    data=json.loads(result.read_text())
    for row in data['rpms']:
        name=row['identity'].split('\t')[0]
        rows.append(dict(arch=arch,name=name,identity=row['identity'].replace('\t',' '),path=row['path'],sha256=row['sha256'],bytes=row['bytes'],payload=row['payload'],file_count=row['file_count'],files=f'verify-{arch}/{name}.files.txt'))
        for line in (E/f'verify-{arch}/{name}.requires.txt').read_text().splitlines():req.append(dict(arch=arch,name=name,requirement=line))
for name,data,fields in [('RPM_INVENTORY.tsv',rows,['arch','name','identity','path','sha256','bytes','payload','file_count','files']),('RPM_REQUIRES.tsv',req,['arch','name','requirement'])]:
    with (E/name).open('w') as f:
        w=csv.DictWriter(f,fieldnames=fields,delimiter='\t');w.writeheader();w.writerows(data)
print('RECIPE_SNAPSHOT_AND_DIFF_WRITTEN',len(rows),'RPM records')
