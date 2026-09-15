"""Read local RPM payload lists and inspect the separate static-build branch's small input."""
import csv
import hashlib
import io
import json
from pathlib import Path
import shlex
import subprocess
import tarfile

p=Path('progress/BUILD_NIGHT_0916')
tmp=Path('tmp/BUILD_NIGHT_0916.h4taGY')
j=(p/'static_packages_commands.jsonl').open('w',buffering=1)
def run(args,binary=False):
 r=subprocess.run(args,capture_output=True)
 record=dict(command=shlex.join(args),exitcode=r.returncode,
             stdout={'bytes':len(r.stdout),'sha256':hashlib.sha256(r.stdout).hexdigest()} if binary else r.stdout.decode(errors='replace'),
             stderr=r.stderr.decode(errors='replace'))
 j.write(json.dumps(record)+'\n')
 assert r.returncode==0,(args,r.returncode)
 return r.stdout

rpms={
 'armv7l':list(Path('tmp/BUILD_W2C_0915/bcc-libcxx-repo/armv7l').glob('bcc-tools-static-*.rpm')),
 'aarch64':list(Path('tmp/BUILD_W2C_0915/bcc-libcxx-repo/aarch64').glob('bcc-tools-static-*.rpm')),
 'x86_64':list(Path('tmp/GBS-ROOT/BUILD-W2-bcc-x86-libcxx/local/repos/tizen_base_standard/x86_64/RPMS').glob('bcc-tools-static-*.rpm')),
}
records=[]
for arch,paths in rpms.items():
 assert len(paths)==1,(arch,paths)
 rpm=paths[0]
 data=rpm.read_bytes()
 meta=run(['rpm','-qp','--qf','%{NAME}\t%{VERSION}-%{RELEASE}\t%{ARCH}\t%{VCS}\n',str(rpm)]).decode()
 files=run(['rpm','-qpl',str(rpm)]).decode()
 records.append(dict(arch=arch,path=str(rpm),sha256=hashlib.sha256(data).hexdigest(),rpm=meta.strip(),files=files.splitlines()))

repo=tmp/'bpftrace-push'
revision=run(['git','-C',str(repo),'rev-parse','FETCH_HEAD']).decode().strip()
assert revision=='624f68d5f67a673083e5ad08ea205742613d6e53'
tarbytes=run(['git','-C',str(repo),'show',revision+':packaging/prebuilt.tar.gz'],binary=True)
members=[]
(tmp/'static_branch_prebuilt').mkdir(exist_ok=True)
with tarfile.open(fileobj=io.BytesIO(tarbytes),mode='r:gz') as archive:
 for n,member in enumerate(archive):
  row=dict(path=member.name,size=member.size,type=member.type.decode())
  if member.isfile():
   body=archive.extractfile(member).read()
   row['sha256']=hashlib.sha256(body).hexdigest()
   # Read each regular member into a numbered local file, never trust tar paths.
   target=tmp/'static_branch_prebuilt'/f'{n}.payload'
   target.write_bytes(body)
   row['local_path']=str(target)
   row['file']=run(['file',str(target)]).decode().strip()
   if member.name.endswith('.a'):
    symbols=run(['nm','-u',str(target)]).decode(errors='replace')
    (p/f'static_branch_archive_{n}.undefined_symbols.txt').write_text(symbols)
    row['std1_lines']=sum('St3__1' in x for x in symbols.splitlines())
    row['stdcxx11_lines']=sum('__cxx11' in x for x in symbols.splitlines())
    row['member_headers']=run(['objdump','-f',str(target)]).decode(errors='replace')
  members.append(row)
result=dict(bcc_static_rpms=records,static_branch=revision,
            prebuilt_tar_sha256=hashlib.sha256(tarbytes).hexdigest(),members=members,
            source1002_origin='NOT_CONFIRMED: this is a different input tar, not the two bpftrace programs')
(p/'static_packages.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(dict(bcc_static_rpms=records,static_branch=revision,
                     prebuilt_members=[{k:v for k,v in r.items() if k!='member_headers'} for r in members]),ensure_ascii=False,indent=2))
