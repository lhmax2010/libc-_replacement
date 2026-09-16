import gzip,hashlib,json,shutil
from pathlib import Path

p=Path('progress/BUILD_STATIC_0917'); d=Path('docs/progress/BUILD_STATIC_0917')
inputs=p/'inputs'; inputs.mkdir(exist_ok=True)
shutil.copy2('tmp/STATIC_0917/condition_cancel_probe.cpp',inputs/'condition_cancel_probe.cpp')
# Public downloaded configuration and API evidence. Do not publish binaries or roots.
public=inputs/'community'; public.mkdir(exist_ok=True)
for src in Path('tmp/STATIC_0917/community').iterdir():
    if src.is_file(): shutil.copy2(src,public/src.name)
# Keep exact current packaging input used for the unchanged fallback checks.
shutil.copy2('tmp/NIGHT_0917/specs/bpftrace.spec',inputs/'bpftrace.previous_candidate.spec')
shutil.copy2('tmp/NIGHT_0917/specs/bpftrace.baseline.spec',inputs/'bpftrace.baseline.spec')
# Preserve large full command/symbol evidence without committing its uncompressed bulk.
compressed={}
for src in p.rglob('*'):
    if not src.is_file() or '__pycache__' in src.parts or src.name=='SHA256SUMS.tsv': continue
    rel=src.relative_to(p); dest=d/rel; dest.parent.mkdir(parents=True,exist_ok=True)
    if src.stat().st_size>2_000_000:
        data=src.read_bytes(); dest=dest.with_name(dest.name+'.gz')
        dest.write_bytes(gzip.compress(data,mtime=0))
        compressed[str(rel)]=dict(archive=str(dest.relative_to(d)),original_sha256=hashlib.sha256(data).hexdigest(),original_bytes=len(data))
    else: shutil.copy2(src,dest)
(d/'COMPRESSED_FILES.json').write_text(json.dumps(compressed,indent=2))
rows=['sha256\tbytes\tpath']
for f in sorted(d.rglob('*')):
    if f.is_file() and f.name!='SHA256SUMS.tsv': rows.append(hashlib.sha256(f.read_bytes()).hexdigest()+'\t'+str(f.stat().st_size)+'\t'+str(f.relative_to(d)))
(d/'SHA256SUMS.tsv').write_text('\n'.join(rows)+'\n')
print('Archived',len(rows)-1,'files to',d)
