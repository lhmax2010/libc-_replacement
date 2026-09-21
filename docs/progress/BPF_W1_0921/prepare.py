"""Freeze complete LLVM inputs and unchanged candidate recipe in an isolated task tree."""
import hashlib,json,os,shlex,subprocess,sys
from pathlib import Path
ROOT=Path.cwd(); OUT=ROOT/'progress/BPF_W1_0921'; TASK=ROOT/'tmp/BPF_W1_0921'; OLD=ROOT/'tmp/WEEKEND_0918'
arch=sys.argv[1]; assert arch in ('armv7l','aarch64')
records=[]
def run(cmd):
    r=subprocess.run(cmd,capture_output=True,text=True,errors='replace')
    records.append(dict(command=shlex.join(cmd),exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr))
    (OUT/f'prepare-{arch}-commands.json').write_text(json.dumps(records,indent=2))
    print(shlex.join(cmd),'exitcode',r.returncode,flush=True)
    if r.returncode: print(r.stdout,r.stderr,flush=True); raise SystemExit(r.returncode)
    return r.stdout
def sha(f):
    h=hashlib.sha256()
    with f.open('rb') as s:
        for b in iter(lambda:s.read(1048576),b''): h.update(b)
    return h.hexdigest()
TASK.mkdir(exist_ok=True); (TASK/'specs').mkdir(exist_ok=True)
specs=[]
for name in ('bpftrace.spec','bpftrace-undefined-harness.spec'):
    src=OLD/'specs'/name; dst=TASK/'specs'/name
    if not dst.exists(): run(['cp','-a',str(src),str(dst)])
    assert sha(src)==sha(dst)
    specs.append(dict(source=str(src),copy=str(dst),sha256=sha(dst)))
(OUT/'unchanged-specs.json').write_text(json.dumps(specs,indent=2))
manifest=ROOT/('progress/ARM_W5_0921/RPM_RESULTS.json' if arch=='armv7l' else 'progress/BUILD_STATIC_0917B/resume_0917/verify_aarch64/result.json')
rows=json.loads(manifest.read_text())['rpms']; assert len(rows)==(22 if arch=='armv7l' else 12)
dest=TASK/'llvm-rpms'/arch; dest.mkdir(parents=True,exist_ok=False)
copied=[]; selected={'llvm','llvm-devel','llvm-static-devel','libllvm','clang','clang-devel','libomp','libomp-devel'}
install=[]
for row in rows:
    src=ROOT/row['path']; dst=dest/src.name
    assert sha(src)==row['sha256'],src
    run(['cp','-a',str(src),str(dst)])
    assert sha(dst)==row['sha256']
    run(['rpm','-K','--nosignature',str(dst)])
    identity=run(['rpm','-qp','--qf','%{NAME}\t%{VERSION}-%{RELEASE}\t%{ARCH}',str(dst)])
    entry=dict(source=str(src),path=str(dst),sha256=row['sha256'],bytes=dst.stat().st_size,identity=identity)
    copied.append(entry)
    if identity.split('\t')[0] in selected: install.append(entry)
(OUT/f'llvm-{arch}-copied.json').write_text(json.dumps(dict(source_manifest=str(manifest),status='COMPLETE_RPM_SET',rpms=copied),indent=2))
assert len(install)==8
src=OLD/f'input-{arch}-v3'; dst=TASK/src.name
assert not dst.exists()
run(['cp','-a',str(src),str(dst)])
verified=[]
for f in sorted(src.rglob('*')):
    g=dst/f.relative_to(src)
    if f.is_symlink(): assert g.is_symlink() and os.readlink(f)==os.readlink(g)
    elif f.is_file():
        a=sha(f); assert a==sha(g),f
        verified.append(dict(path=str(f.relative_to(src)),sha256=a,bytes=f.stat().st_size))
        if len(verified)%500==0: print('COPY_ENTRY',len(verified),flush=True)
(OUT/f'private-{arch}-copy.json').write_text(json.dumps(dict(source=str(src),copy=str(dst),files=verified),indent=2))
prior=json.loads((ROOT/f'progress/BUILD_WEEKEND_0918/install-v3-{arch}-inputs.json').read_text())
for row in prior:
    if row['identity'].split('\t')[0] in selected: continue
    f=Path(row['path']); assert sha(f)==row['sha256']
    run(['rpm','-K','--nosignature',str(f)])
    install.append(row)
base=json.loads((ROOT/f'progress/BUILD_WEEKEND_0918/bpf-private-{arch}-base.json').read_text())
base=[x.replace(str(OLD),str(TASK)) for x in base]
guest=[]
for i,row in enumerate(install):
    g=f'/tmp/input-rpms/{i}.rpm'; guest.append(g); base+=['--ro-bind',row['path'],g]
(OUT/f'installed-{arch}-inputs.json').write_text(json.dumps(install,indent=2))
run(base+['--','rpm','-Uvh','--ignorearch','--oldpackage','--replacepkgs','--test',*guest])
run(base+['--','rpm','-Uvh','--ignorearch','--oldpackage','--replacepkgs',*guest])
libdir='lib' if arch=='armv7l' else 'lib64'
archive=dst/'usr'/libdir/'libclang.a'
archive_sha=sha(archive)
if arch=='armv7l': assert archive_sha=='5deb3c6c662c73865e8a38f4f8df8d6e672af2ae8a6a8faae7700b5909c1524a'
run(base+['--','rpm','-qa','--qf','%{NAME}\t%{VERSION}-%{RELEASE}\t%{ARCH}\n'])
(OUT/f'prepare-{arch}-result.json').write_text(json.dumps(dict(status='PASS',llvm_rpms=len(copied),libclang_sha256=archive_sha),indent=2))
