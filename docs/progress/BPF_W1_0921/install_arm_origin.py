"""Install the already declared inputs normally into a fresh, task-owned copy."""
import hashlib,json,os,shlex,stat,subprocess
from pathlib import Path
p=Path.cwd(); out=p/'progress/BPF_W1_0921'; copy=out/'origin-copy'
dest=p/'tmp/BPF_W1_0921/input-armv7l-origin'
assert json.loads((out/'original-arm-test-result.json').read_text())['status']=='NORMAL_TRANSACTION_TEST_PASS'
assert json.loads((copy/'copy-armv7l-result.json').read_text())['status']=='COPY_AND_PER_FILE_SHA256_PASS'
assert not (out/'prepare-armv7l-result.json').exists()
changes=[]
for part in ('usr','etc','rpmdb','root'):
    for directory,dirs,files in os.walk(dest/part,followlinks=False):
        f=Path(directory); info=f.lstat()
        assert info.st_uid==os.getuid() and not f.is_symlink()
        before=stat.S_IMODE(info.st_mode); after=before|stat.S_IWUSR
        if before!=after:
            os.chmod(f,after); changes.append(dict(path=str(f),before=oct(before),after=oct(after)))
(out/'origin-directory-permissions.json').write_text(json.dumps(changes,indent=2))
base=json.loads((copy/'bpf-private-armv7l-base.json').read_text())
rows=json.loads((out/'installed-armv7l-inputs.json').read_text()); guest=[]; records=[]
def sha(f):
    with f.open('rb') as s: return hashlib.file_digest(s,'sha256').hexdigest()
def run(cmd):
    r=subprocess.run(cmd,capture_output=True,text=True,errors='replace')
    records.append(dict(command=shlex.join(cmd),exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr))
    (out/'origin-install-commands.json').write_text(json.dumps(records,indent=2))
    print(shlex.join(cmd),'exitcode',r.returncode,flush=True); print(r.stdout,r.stderr,flush=True)
    assert r.returncode==0
for i,row in enumerate(rows):
    assert sha(Path(row['path']))==row['sha256']
    g=f'/tmp/input-rpms/{i}.rpm'; guest.append(g); base+=['--ro-bind',row['path'],g]
run(base+['--','rpm','-Uvh','--ignorearch','--oldpackage','--replacepkgs','--test',*guest])
run(base+['--','rpm','-Uvh','--ignorearch','--oldpackage','--replacepkgs',*guest])
digest=sha(dest/'usr/lib/libclang.a')
assert digest=='5deb3c6c662c73865e8a38f4f8df8d6e672af2ae8a6a8faae7700b5909c1524a'
run(base+['--','rpm','-qa','--qf','%{NAME}\t%{VERSION}-%{RELEASE}\t%{ARCH}\n'])
(out/'prepare-armv7l-result.json').write_text(json.dumps(dict(status='PASS',llvm_rpms=22,libclang_sha256=digest,input_root=str(dest),replacefiles_used=False,original_root_modified=False,earlier_failed_copy_preserved=True),indent=2))
print('NORMAL_TRANSACTION_INSTALL_PASS',digest,flush=True)
