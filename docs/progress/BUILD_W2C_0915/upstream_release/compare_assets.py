"""Hash public release assets and inspect ELF layers; do not infer identity from versions."""
import csv
import hashlib
import json
import re
import shlex
import subprocess
import tarfile
from pathlib import Path
p=Path('progress/BUILD_W2C_0915/upstream_release')
downloads=Path('tmp/BUILD_W2C_0915/upstream-release.yvCwWp')
j=(p/'comparison_commands.jsonl').open('w',buffering=1)
def digest(path):
    h=hashlib.sha256()
    with path.open('rb') as f:
        for block in iter(lambda:f.read(1024*1024),b''):h.update(block)
    return h.hexdigest()
def run(args,allowed=(0,)):
    r=subprocess.run(args,capture_output=True,text=True,errors='replace')
    j.write(json.dumps(dict(command=shlex.join(args),exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr))+'\n')
    assert r.returncode in allowed,(args,r.returncode)
    return r.stdout
release=json.loads((p/'release_v0.24.2.json').read_text())
assets=[]
for meta in release['assets']:
    name=meta['name'];path=downloads/name
    binary_content=name=='bpftrace' or name.startswith('binary_tools_man-bundle.')
    row=dict(name=name,size=meta['size'],github_digest=meta.get('digest'),url=meta['browser_download_url'],
        downloaded=binary_content)
    if binary_content:
        assert path.stat().st_size==meta['size']
        row['sha256']=digest(path)
        assert 'sha256:'+row['sha256']==meta['digest']
    assets.append(row)
bundles=[]
for name in ['binary_tools_man-bundle.tar.xz','binary_tools_man-bundle.tar.zst']:
    path=downloads/name
    proc=None
    if name.endswith('.zst'):
        command=['zstd','-dc',str(path)]
        proc=subprocess.Popen(command,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        archive=tarfile.open(fileobj=proc.stdout,mode='r|')
    else:archive=tarfile.open(path,mode='r:xz')
    members=[];binary=[]
    with archive:
        for member in archive:
            members.append(dict(name=member.name,size=member.size,type=member.type.decode()))
            if member.isfile() and member.name.endswith('/bpftrace'):
                h=hashlib.sha256();f=archive.extractfile(member)
                for block in iter(lambda:f.read(1024*1024),b''):h.update(block)
                binary.append(dict(member=member.name,size=member.size,sha256=h.hexdigest()))
    if proc:
        # Drain decompressor padding; record its command/exit without dumping binary stdout.
        rest=proc.stdout.read();stderr=proc.stderr.read().decode();rc=proc.wait()
        j.write(json.dumps(dict(command=shlex.join(command),exitcode=rc,stdout='binary tar stream consumed by tarfile',stderr=stderr))+'\n')
        assert rc==0
    assert len(binary)==1 and binary[0]['sha256']==assets[2]['sha256']
    (p/(name+'.members.json')).write_text(json.dumps(members,indent=2)+'\n')
    bundles.append(dict(asset=name,member_count=len(members),binary_members=binary))

inner=list((downloads/'appimage-root/nix/store').glob('*-bpftrace/bin/bpftrace'))
assert len(inner)==1
targets=[('official_appimage',downloads/'bpftrace'),('official_inner_bpftrace',inner[0]),
    ('source1002_armv7l',Path('tmp/BUILD_W2C_0915/bpf-prebuilt.vuDDCU/prebuilt-static-bpftrace/arm/usr/bin/bpftrace')),
    ('source1002_aarch64',Path('tmp/BUILD_W2C_0915/bpf-prebuilt.vuDDCU/prebuilt-static-bpftrace/aarch64/usr/bin/bpftrace'))]
rows=[]
for label,path in targets:
    description=run(['file',str(path)]).strip()
    header=run(['readelf','-h',str(path)])
    sections=run(['readelf','-SW',str(path)])
    dynamic=run(['readelf','-d',str(path)])
    regular=run(['nm','--defined-only',str(path)])
    dyn=run(['nm','-D',str(path)])
    (p/f'{label}.readelf.txt').write_text(header+sections+dynamic)
    (p/f'{label}.nm.txt').write_text(regular)
    (p/f'{label}.dynsym.txt').write_text(dyn)
    rows.append(dict(object=label,size=path.stat().st_size,sha256=digest(path),file=description,
        machine=re.search(r'Machine:\s*(.*)',header).group(1),needed=re.findall(r'Shared library: \[(.*?)\]',dynamic),
        regular_symbol_lines=len(regular.splitlines()),dynamic_symbol_lines=len(dyn.splitlines()),
        std1_dynamic_lines=sum('_ZNSt3__1' in line for line in dyn.splitlines()),
        stdcxx11_dynamic_lines=sum('__cxx11' in line for line in dyn.splitlines())))
for row in rows[2:]:
    row['matches_official_outer']=row['sha256']==rows[0]['sha256']
    row['matches_official_inner']=row['sha256']==rows[1]['sha256']
assert not any(r['matches_official_outer'] or r['matches_official_inner'] for r in rows[2:])
runtime=[]
for path in (downloads/'appimage-root/nix/store').glob('*/lib/libstdc++.so.*'):
    if path.is_symlink():continue
    runtime.append(dict(path=str(path),sha256=digest(path),file=run(['file',str(path)]).strip()))
result=dict(assets=assets,bundles=bundles,elf_comparison=rows,official_bundled_libstdcxx=runtime,
    match_status='NO_MATCH_IN_CURRENT_V0_24_2_RELEASE_ASSETS',
    same_application_version='bpftrace v0.24.2; see actual --version records',
    scope='Current published release assets and their bundled bpftrace only; not all GitHub Actions artifacts or withdrawn assets.')
(p/'comparison.json').write_text(json.dumps(result,indent=2)+'\n')
with (p/'sha256_comparison.tsv').open('w') as f:
    keys=['object','size','sha256','machine','needed','regular_symbol_lines','dynamic_symbol_lines']
    w=csv.DictWriter(f,delimiter='\t',fieldnames=keys);w.writeheader()
    for row in rows:w.writerow({k:row[k] for k in keys})
print(json.dumps(result,indent=2))
