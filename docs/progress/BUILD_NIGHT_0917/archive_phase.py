import gzip,hashlib,shutil,sys
from pathlib import Path
src=Path('progress/BUILD_NIGHT_0917'); dst=Path('docs/progress/BUILD_NIGHT_0917'); active=sys.argv[1]
snap=src/'candidate_specs'; snap.mkdir(exist_ok=True)
for p in Path('tmp/NIGHT_0917/specs').glob('*'):
    if p.is_file(): shutil.copy2(p,snap/p.name)
rows=[]; compressed=[]
for p in sorted(src.rglob('*')):
    if not p.is_file() or p.name=='SHA256SUMS.tsv': continue
    rel=p.relative_to(src)
    if rel.parts[0]=='raw' and p.name.startswith(active+'.'): continue
    data=p.read_bytes()
    if p.name.startswith('bpftrace-') and len(data)>5*1024*1024 and p.suffix!='.gz':
        dest_rel=Path(str(rel)+'.gz'); packed=gzip.compress(data,compresslevel=6,mtime=0)
        q=dst/dest_rel; q.parent.mkdir(parents=True,exist_ok=True); q.write_bytes(packed)
        assert gzip.decompress(q.read_bytes())==data
        compressed.append((str(rel),len(data),hashlib.sha256(data).hexdigest(),str(dest_rel)))
        rows.append((str(dest_rel),len(packed),hashlib.sha256(packed).hexdigest()))
    else:
        q=dst/rel; q.parent.mkdir(parents=True,exist_ok=True); shutil.copy2(p,q)
        assert q.read_bytes()==data
        rows.append((str(rel),len(data),hashlib.sha256(data).hexdigest()))
meta='source_path\tsource_bytes\tsource_sha256\tarchived_path\n'+''.join(f'{r}\t{n}\t{h}\t{a}\n' for r,n,h,a in compressed)
(src/'COMPRESSED_OUTPUTS.tsv').write_text(meta); (dst/'COMPRESSED_OUTPUTS.tsv').write_text(meta)
rows=[r for r in rows if r[0]!='COMPRESSED_OUTPUTS.tsv']
rows.append(('COMPRESSED_OUTPUTS.tsv',len(meta.encode()),hashlib.sha256(meta.encode()).hexdigest()))
data='path\tbytes\tsha256\n'+''.join(f'{p}\t{s}\t{h}\n' for p,s,h in rows)
(src/'SHA256SUMS.tsv').write_text(data); (dst/'SHA256SUMS.tsv').write_text(data)
print('Archived',len(rows),'files; binary assets remain in tmp; active wrapper excluded until next phase.')
