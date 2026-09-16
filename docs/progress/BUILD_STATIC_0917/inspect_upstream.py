import hashlib, json
from pathlib import Path
from probes import p, run
out=Path('tmp/STATIC_0917/community')
files=['.github/include/static.sh','.github/workflows/static.yml','.github/workflows/release.yml','docker/Dockerfile.static','flake.nix']
manifest=[]
for i,path in enumerate(files):
    url='https://raw.githubusercontent.com/bpftrace/bpftrace/v0.24.2/'+path
    dest=out/('ci-'+str(i)+'.txt')
    r=run('upstream-ci-'+str(i),['curl','-sS','-L','--max-time','40','-o',str(dest),'-w','%{http_code}\n',url],50)
    manifest.append(dict(path=path,url=url,download=str(dest),http=r['stdout'].strip(),sha256=hashlib.sha256(dest.read_bytes()).hexdigest()))
for number in [1710,342,4233,4243]:
    url=f'https://api.github.com/repos/bpftrace/bpftrace/issues/{number}/comments?per_page=100'
    dest=out/('issue-'+str(number)+'-comments.json')
    r=run('upstream-comments-'+str(number),['curl','-sS','-L','--max-time','40','-o',str(dest),'-w','%{http_code}\n',url],50)
    manifest.append(dict(url=url,download=str(dest),http=r['stdout'].strip(),sha256=hashlib.sha256(dest.read_bytes()).hexdigest()))
(p/'upstream_more_sources.json').write_text(json.dumps(manifest,indent=2))
root=Path('tmp/BUILD_W2C_0915/upstream-release.yvCwWp/appimage-root/nix/store')
binary=list(root.glob('*-bpftrace/bin/bpftrace'))
assert len(binary)==1
run('official-appimage-inner-needed',['readelf','-d',str(binary[0])])
run('official-appimage-inner-sha256',['sha256sum',str(binary[0])])
