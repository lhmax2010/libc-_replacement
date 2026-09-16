import hashlib,shutil,sys
from pathlib import Path
src=Path('progress/BUILD_STATIC_0916B'); dst=Path('docs/progress/BUILD_STATIC_0916B')
active=sys.argv[1]
(src/'inputs').mkdir(exist_ok=True)
for f in [Path('tmp/NIGHT_0917/finish_validation/condition_cancel_probe.cpp'), *[Path('tmp/NIGHT_0917/specs')/n for n in ['libcxx-runtimes.spec','llvm.spec','bpftrace.spec']]]:
    shutil.copy2(f,src/'inputs'/f.name)
rows=[]
for f in sorted(src.rglob('*')):
    if not f.is_file() or f.name=='SHA256SUMS.tsv': continue
    rel=f.relative_to(src)
    if rel.parts[0]=='raw' and f.name.startswith(active+'.'): continue
    target=dst/rel; target.parent.mkdir(parents=True,exist_ok=True); shutil.copy2(f,target)
    data=f.read_bytes(); assert target.read_bytes()==data
    rows.append(f'{rel}\t{len(data)}\t{hashlib.sha256(data).hexdigest()}\n')
manifest='path\tbytes\tsha256\n'+''.join(rows)
(src/'SHA256SUMS.tsv').write_text(manifest); (dst/'SHA256SUMS.tsv').write_text(manifest)
print('Archived',len(rows),'files; active wrapper archived in next pass; RPMs/binaries retained only in tmp.')
