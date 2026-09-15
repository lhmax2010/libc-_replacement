"""Read-only input inspection; generated tables are task evidence, not package edits."""
import csv
import hashlib
import json
import re
import shlex
import subprocess
import tarfile
from pathlib import Path

p=Path('progress/BUILD_W2C_0915/static_origin')
j=(p/'commands.jsonl').open('w',buffering=1)
def run(args, allowed=(0,)):
    r=subprocess.run(args,capture_output=True,text=True,errors='replace')
    j.write(json.dumps(dict(command=shlex.join(args),exitcode=r.returncode,
        stdout=r.stdout,stderr=r.stderr),ensure_ascii=False)+'\n')
    assert r.returncode in allowed,(args,r.returncode)
    return r.stdout
def sha(data):return hashlib.sha256(data).hexdigest()
def tsv(name,rows):
    with (p/name).open('w') as f:
        w=csv.DictWriter(f,delimiter='\t',fieldnames=list(rows[0]));w.writeheader();w.writerows(rows)

# Inventory and search every locally present ref tip, plus history searches in raw/004.
repo='codes/qemu-accel'
refs=run(['git','-C',repo,'for-each-ref','--format=%(objectname)','refs/heads','refs/remotes']).splitlines()
tips=sorted(set(refs))
hits=[]
for tip in tips:
    result=run(['git','-C',repo,'grep','-n','-i','-E','bpftrace|prebuilt-static|libbpf',tip],(0,1))
    if result:hits.append(dict(revision=tip,output=result))
qemu_files=[]
for name in run(['git','-C',repo,'ls-files']).splitlines():
    path=Path(repo)/name
    qemu_files.append(dict(path=name,size=path.stat().st_size,sha256=sha(path.read_bytes())))
tsv('qemu_source_files.tsv',qemu_files)

# Match exact binary package names, not substrings, and verify original download hashes.
provenance=Path('docs/progress/R98/raw/010_fetch_llvm_snapshot_metadata_final.stdout').read_text()
records={}
for line in provenance.splitlines():
    fields=line.split('\t')
    if len(fields)==5 and fields[0]=='FETCH' and '/image_packages/' in fields[2]:
        records[Path(fields[2]).name]=dict(url=fields[1],size=int(fields[3]),sha256=fields[4])
images=[];image_hits=[]
manifests=sorted(Path('docs/progress/R98/inputs/image_packages').glob('*.packages'))
assert len(manifests)==6
for path in manifests:
    data=path.read_bytes();origin=records[path.name]
    assert len(data)==origin['size'] and sha(data)==origin['sha256']
    counts={name:0 for name in ['bpftrace','bpftrace-static','qemu-accel']}
    lines=data.decode().splitlines()
    for n,line in enumerate(lines,1):
        fields=line.split()
        assert len(fields)==3,(path,n,line)
        name,arch=fields[0].rsplit('.',1)
        is_qemu=name=='qemu-accel' or name.startswith('qemu-accel-')
        if name in ('bpftrace','bpftrace-static') or is_qemu:
            counts['qemu-accel' if is_qemu else name]+=1
            image_hits.append(dict(manifest=path.name,line=n,name=name,arch=arch,version=fields[1],vcs=fields[2],raw=line))
    images.append(dict(manifest=path.name,entries=len(lines),sha256=sha(data),url=origin['url'],
        bpftrace=counts['bpftrace'],bpftrace_static=counts['bpftrace-static'],qemu_accel_family=counts['qemu-accel']))
tsv('image_membership.tsv',images)
if image_hits:tsv('image_hits.tsv',image_hits)
else:(p/'image_hits.tsv').write_text('manifest\tline\tname\tarch\tversion\tvcs\traw\n')
# Positive control: literal names with prefix/suffix decoys.
control=['bpftrace.armv7l','bpftrace-static.armv7l','bpftrace-static-devel.armv7l','x-bpftrace-static.armv7l']
assert [item.rsplit('.',1)[0]=='bpftrace-static' for item in control]==[False,True,False,False]

archive=Path('codes/R104/bpftrace/packaging/prebuilt-static-bpftrace.tar.gz')
archive_hash=sha(archive.read_bytes())
assert archive_hash=='3785365b768e29a2f6a31a54b5f1843b3fc64d974ecc7dd588197644d3514123'
binaries=[]
with tarfile.open(archive) as tar:
    for member in tar.getmembers():
        if not member.isfile():continue
        data=tar.extractfile(member).read()
        assert data.startswith(b'\x7fELF'),member.name
        original=Path('tmp/BUILD_W2C_0915/bpf-prebuilt.vuDDCU')/member.name
        assert sha(original.read_bytes())==sha(data)
        description=run(['file',str(original)])
        header=run(['readelf','-h',str(original)])
        dynamic=run(['readelf','-d',str(original)])
        strings=run(['strings','-a',str(original)])
        selected=[]
        patterns={'mangled_std1':r'_ZNSt3__1','mangled_stdcxx11':r'_ZNSt7__cxx11',
                  'readable_std1':r'std::__1','readable_stdcxx11':r'std::__cxx11',
                  'version':r'(?i)(?:bpftrace v?\d|clang version|LLVM version|GCC:|GCC version|GNU C\+\+|GLIBCXX_\d)'}
        counts={}
        for category,pattern in patterns.items():
            matched=[line for line in strings.splitlines() if re.search(pattern,line)]
            counts[category]=len(matched)
            selected.extend(dict(kind=category,text=line) for line in matched)
        arch='armv7l' if '/arm/' in member.name else 'aarch64'
        (p/f'{arch}_string_matches.json').write_text(json.dumps(selected,ensure_ascii=False,indent=2)+'\n')
        binaries.append(dict(arch=arch,member=member.name,size=len(data),sha256=sha(data),
            needed=re.findall(r'Shared library: \[(.*?)\]',dynamic),file=description.strip(),
            strings_matches=counts,qemu_counterpart='NOT_AVAILABLE'))
result=dict(qemu_revision=run(['git','-C',repo,'rev-parse','HEAD']).strip(),
    local_ref_records=len(refs),unique_ref_tips=len(tips),ref_tip_hits=hits,source_files=len(qemu_files),
    archive_sha256=archive_hash,binaries=binaries,images=images,image_control='PASS',
    source_origin='NOT_OBSERVED',qemu_payload_sha_comparison='NOT_AVAILABLE: no matching binary/build recipe in supplied source tree')
(p/'summary.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(result,ensure_ascii=False))
