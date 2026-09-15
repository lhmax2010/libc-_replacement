import collections,gzip,hashlib,json,re,sys
from pathlib import Path
p=Path('progress/BUILD_NIGHT_0917'); arch=sys.argv[1]
f=Path('tmp/NIGHT_0917/builds')/f'bpftrace-{arch}-static/bpftrace.map'
data=f.read_bytes(); text=data.decode(errors='replace')
# The initial GNU ld section explicitly records members pulled for references.
section=text.split('Merging program properties',1)[0].split('Discarded input sections',1)[0]
rows=[]
for line in section.splitlines():
    m=re.match(r'^(\S+\.a)\(([^)]+)\)',line)
    if m: rows.append(m.groups())
rows=sorted(set(rows)); counts=collections.Counter(x[0] for x in rows)
(p/f'bpftrace-{arch}-pulled_members.tsv').write_text('archive\tmember\n'+''.join(a+'\t'+m+'\n' for a,m in rows))
selected={a:n for a,n in counts.items() if any(s in a for s in ['libclang.a','libc++.a','libc++abi.a','libgcc_eh.a','libstdc++'])}
r=dict(map_path=str(f),sha256=hashlib.sha256(data).hexdigest(),bytes=len(data),pulled_members=len(rows),selected_archives=selected,stdcppfs_pulled=sum(n for a,n in counts.items() if 'libstdc++fs.a' in a))
(p/f'bpftrace-{arch}-link_map_summary.json').write_text(json.dumps(r,indent=2))
# Preserve the whole map losslessly; never submit the binary itself.
(p/f'bpftrace-{arch}-link.map.gz').write_bytes(gzip.compress(data,compresslevel=6,mtime=0))
print(json.dumps(r,indent=2))
